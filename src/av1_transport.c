#include <smolrtsp/av1_transport.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <slice99.h>

/* AV1 RTP §4.4 W field is 2 bits, but a TU can contain many more
 * elements than that. With this transport's one-OBU-per-packet strategy
 * (W = 1), the only limit on OBUs per TU is the size of the
 * per-call element table. 32 is generous: real-world TUs from libaom,
 * librav1e, and svt-av1 carry 2-5 OBUs. */
#define MAX_OBUS_PER_TU 32

typedef struct {
    /* OBU header bytes with obu_has_size_field cleared (AOMedia AV1
     * RTP §4.5). 1 byte normally, 2 if obu_extension_flag is set. */
    uint8_t prefix[2];
    uint8_t prefix_len;
    /* Payload after the OBU header (and after the obu_size LEB128 if
     * the original OBU had one). */
    U8Slice99 payload;
} Av1ObuElement;

SmolRTSP_Av1TransportConfig SmolRTSP_Av1TransportConfig_default(void) {
    return (SmolRTSP_Av1TransportConfig){
        .max_packet_size = SMOLRTSP_MAX_AV1_PACKET_SIZE,
    };
}

struct SmolRTSP_Av1Transport {
    SmolRTSP_RtpTransport *transport;
    SmolRTSP_Av1TransportConfig config;
};

SmolRTSP_Av1Transport *SmolRTSP_Av1Transport_new(SmolRTSP_RtpTransport *t) {
    return SmolRTSP_Av1Transport_new_with_config(
        t, SmolRTSP_Av1TransportConfig_default());
}

SmolRTSP_Av1Transport *SmolRTSP_Av1Transport_new_with_config(
    SmolRTSP_RtpTransport *t, SmolRTSP_Av1TransportConfig config) {
    assert(t);
    assert(config.max_packet_size > SMOLRTSP_AV1_AGGREGATION_HEADER_SIZE + 1);

    SmolRTSP_Av1Transport *self = malloc(sizeof *self);
    assert(self);

    self->transport = t;
    self->config = config;

    return self;
}

static void SmolRTSP_Av1Transport_drop(VSelf) {
    VSELF(SmolRTSP_Av1Transport);
    assert(self);

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(self->transport);

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_Av1Transport);

bool SmolRTSP_Av1Transport_is_full(SmolRTSP_Av1Transport *self) {
    return SmolRTSP_RtpTransport_is_full(self->transport);
}

/* Parses @p obus into Av1ObuElement entries, dropping Temporal
 * Delimiter (type 2) and Tile List (type 8) OBUs per AOMedia AV1 RTP
 * §5. Returns the number of elements produced, or -1 on parse error
 * (truncated stream, malformed LEB128, or > MAX_OBUS_PER_TU OBUs). */
static int parse_obus(U8Slice99 obus, Av1ObuElement *out, size_t max_elements) {
    size_t pos = 0;
    size_t n = 0;

    while (pos < obus.len) {
        const uint8_t hdr = obus.ptr[pos++];
        const bool has_ext = smolrtsp_av1_obu_has_extension(hdr);
        const bool has_size = smolrtsp_av1_obu_has_size_field(hdr);
        const uint8_t obu_type = smolrtsp_av1_obu_type(hdr);

        uint8_t ext_byte = 0;
        if (has_ext) {
            if (pos >= obus.len) {
                return -1;
            }
            ext_byte = obus.ptr[pos++];
        }

        uint64_t payload_len;
        if (has_size) {
            const size_t used = smolrtsp_av1_leb128_decode(
                obus.ptr + pos, obus.len - pos, &payload_len);
            if (used == 0) {
                return -1;
            }
            pos += used;
        } else {
            /* No size field: the OBU runs to the end of the input
             * buffer and must be the last OBU in the stream (AV1 spec
             * §5.3.2). */
            payload_len = obus.len - pos;
        }

        if (pos + payload_len > obus.len) {
            return -1;
        }

        if (obu_type == SMOLRTSP_AV1_OBU_TEMPORAL_DELIMITER ||
            obu_type == SMOLRTSP_AV1_OBU_TILE_LIST) {
            pos += (size_t)payload_len;
            continue;
        }

        if (n >= max_elements) {
            return -1;
        }

        Av1ObuElement *e = &out[n++];
        e->prefix[0] = smolrtsp_av1_obu_clear_size_field(hdr);
        e->prefix_len = 1;
        if (has_ext) {
            e->prefix[1] = ext_byte;
            e->prefix_len = 2;
        }
        e->payload = U8Slice99_new(obus.ptr + pos, (size_t)payload_len);
        pos += (size_t)payload_len;
    }

    return (int)n;
}

/* Sends one element across one or more RTP packets, fragmenting if
 * needed. Sets Z/Y per AOMedia AV1 RTP §4.4. Sets N on the first
 * chunk if @p set_n_first is true. Sets the RTP marker on the last
 * chunk if @p set_marker_last is true. */
static int send_element(
    SmolRTSP_Av1Transport *self, SmolRTSP_RtpTimestamp ts,
    const Av1ObuElement *e, bool set_n_first, bool set_marker_last) {
    const size_t header_budget = SMOLRTSP_AV1_AGGREGATION_HEADER_SIZE;
    const size_t per_packet_budget =
        self->config.max_packet_size - header_budget;
    const size_t total_bytes = (size_t)e->prefix_len + e->payload.len;

    size_t bytes_left = total_bytes;
    size_t payload_consumed = 0;
    bool first_chunk = true;

    while (bytes_left > 0) {
        const size_t chunk_size =
            bytes_left <= per_packet_budget ? bytes_left : per_packet_budget;
        const bool is_last_chunk = (chunk_size == bytes_left);

        size_t prefix_in_chunk = 0;
        if (first_chunk) {
            /* The entire prefix must fit in the first chunk so the
             * receiver sees a complete OBU header. With a 1200-byte
             * MTU and a 2-byte prefix this is trivially true; we
             * still assert it to fail loudly on absurd configs. */
            assert(per_packet_budget >= e->prefix_len);
            prefix_in_chunk = e->prefix_len;
        }
        const size_t payload_in_chunk = chunk_size - prefix_in_chunk;

        const SmolRTSP_Av1AggregationHeader agg = {
            .z = !first_chunk,
            .y = !is_last_chunk,
            .w = 1,
            .n = first_chunk && set_n_first,
        };

        /* Build the packet's payload header: the aggregation byte,
         * followed by the OBU prefix bytes only on the first chunk. */
        uint8_t pkt_hdr[SMOLRTSP_AV1_AGGREGATION_HEADER_SIZE + 2];
        size_t pkt_hdr_len = 0;
        SmolRTSP_Av1AggregationHeader_serialize(agg, pkt_hdr);
        pkt_hdr_len = SMOLRTSP_AV1_AGGREGATION_HEADER_SIZE;
        if (first_chunk) {
            for (size_t i = 0; i < e->prefix_len; i++) {
                pkt_hdr[pkt_hdr_len++] = e->prefix[i];
            }
        }

        const U8Slice99 chunk_payload =
            U8Slice99_new(e->payload.ptr + payload_consumed, payload_in_chunk);
        const bool marker = is_last_chunk && set_marker_last;

        if (SmolRTSP_RtpTransport_send_packet(
                self->transport, ts, marker,
                U8Slice99_new(pkt_hdr, pkt_hdr_len), chunk_payload) == -1) {
            return -1;
        }

        bytes_left -= chunk_size;
        payload_consumed += payload_in_chunk;
        first_chunk = false;
    }

    return 0;
}

int SmolRTSP_Av1Transport_send_temporal_unit(
    SmolRTSP_Av1Transport *self, SmolRTSP_RtpTimestamp ts, U8Slice99 obus,
    bool is_new_cvs) {
    assert(self);

    Av1ObuElement elements[MAX_OBUS_PER_TU];
    const int n = parse_obus(obus, elements, MAX_OBUS_PER_TU);
    if (n < 0) {
        return -1;
    }
    if (n == 0) {
        /* TU was just a TD or empty -- nothing to send on the wire. */
        return 0;
    }

    for (int i = 0; i < n; i++) {
        const bool is_last_obu = (i == n - 1);
        const bool set_n = (i == 0) && is_new_cvs;
        if (send_element(self, ts, &elements[i], set_n, is_last_obu) == -1) {
            return -1;
        }
    }

    return 0;
}
