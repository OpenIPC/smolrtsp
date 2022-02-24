#include <smolrtsp/nal_transport.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include <alloca.h>

#include <slice99.h>

static int send_fragmentized_nal_data(
    SmolRTSP_RtpTransport *t, uint64_t timestamp_us, uint8_t payload_type,
    size_t max_packet_size, SmolRTSP_NalUnit nal_unit);
static int send_fu(
    SmolRTSP_RtpTransport *t, uint64_t timestamp_us, uint8_t payload_type,
    SmolRTSP_NalUnit fu, bool is_first_fragment, bool is_last_fragment);

struct SmolRTSP_NalTransport {
    SmolRTSP_RtpTransport *transport;
};

SmolRTSP_NalTransport *SmolRTSP_NalTransport_new(SmolRTSP_RtpTransport *t) {
    assert(t);

    SmolRTSP_NalTransport *self = malloc(sizeof *self);
    assert(self);

    self->transport = t;

    return self;
}

static void SmolRTSP_NalTransport_drop(VSelf) {
    VSELF(SmolRTSP_NalTransport);
    assert(self);

    VTABLE(SmolRTSP_NalTransport, SmolRTSP_Droppable).drop(self->transport);

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_NalTransport);

// TODO: justify these magic numbers.

#define MAX_H264_PACKET_SIZE 1200
#define MAX_H265_PACKET_SIZE 4096

int SmolRTSP_NalTransport_send_packet(
    SmolRTSP_NalTransport *self, uint64_t timestamp_us, uint8_t payload_type,
    SmolRTSP_NalUnit nal_unit) {
    assert(self);

    SMOLRTSP_NAL_UNIT_DESTRUCT(nal_unit, h, payload);

    const size_t max_packet_size =
        MATCHES(h, SmolRTSP_NalHeader_H264) ? MAX_H264_PACKET_SIZE : MAX_H265_PACKET_SIZE;

    if (payload.len < max_packet_size) {
        const bool marker = SmolRTSP_NalHeader_is_coded_slice_idr(h) ||
                            SmolRTSP_NalHeader_is_coded_slice_non_idr(h);

        const size_t header_buf_size = SmolRTSP_NalHeader_size(h);
        uint8_t *header_buf = alloca(header_buf_size);
        SmolRTSP_NalHeader_serialize(h, header_buf);

        return SmolRTSP_RtpTransport_send_packet(
            self->transport, timestamp_us, marker, payload_type,
            SmolRTSP_NalHeader_clock_rate_kHz(h),
            U8Slice99_new(header_buf, header_buf_size), payload);
    }

    return send_fragmentized_nal_data(
        self->transport, timestamp_us, payload_type, max_packet_size, nal_unit);
}

// See <https://tools.ietf.org/html/rfc6184#section-5.8> (H.264),
// <https://tools.ietf.org/html/rfc7798#section-4.4.3> (H.265).
static int send_fragmentized_nal_data(
    SmolRTSP_RtpTransport *t, uint64_t timestamp_us, uint8_t payload_type,
    size_t max_packet_size, SmolRTSP_NalUnit nal_unit) {
    SMOLRTSP_NAL_UNIT_DESTRUCT(nal_unit, h, payload);

    const size_t rem = payload.len % max_packet_size,
                 packets_count = (payload.len - rem) / max_packet_size;

    for (size_t packet_idx = 0; packet_idx < packets_count; packet_idx++) {
        const bool is_first_fragment = 0 == packet_idx,
                   is_last_fragment = 0 == rem && (packets_count - 1 == packet_idx);

        const U8Slice99 fu_data = U8Slice99_sub(
            payload, packet_idx * max_packet_size,
            is_last_fragment ? payload.len : (packet_idx + 1) * max_packet_size);
        const SmolRTSP_NalUnit fu = SmolRTSP_NalUnit_new(h, fu_data);

        if (send_fu(
                t, timestamp_us, payload_type, fu, is_first_fragment, is_last_fragment) ==
            -1) {
            return -1;
        }
    }

    if (rem != 0) {
        const U8Slice99 fu_data =
            U8Slice99_advance(payload, packets_count * max_packet_size);
        const SmolRTSP_NalUnit fu = SmolRTSP_NalUnit_new(h, fu_data);
        const bool is_first_fragment = 0 == packets_count, is_last_fragment = true;

        if (send_fu(
                t, timestamp_us, payload_type, fu, is_first_fragment, is_last_fragment) ==
            -1) {
            return -1;
        }
    }

    return 0;
}

static int send_fu(
    SmolRTSP_RtpTransport *t, uint64_t timestamp_us, uint8_t payload_type,
    SmolRTSP_NalUnit fu, bool is_first_fragment, bool is_last_fragment) {
    SMOLRTSP_NAL_UNIT_DESTRUCT(fu, h, payload);

    const size_t fu_header_size = SmolRTSP_NalHeader_fu_size(h);
    U8Slice99 fu_header = U8Slice99_new(alloca(fu_header_size), fu_header_size);

    SmolRTSP_NalHeader_write_fu_header(
        h, fu_header.ptr, is_first_fragment, is_last_fragment);

    const bool marker = is_last_fragment;

    return SmolRTSP_RtpTransport_send_packet(
        t, timestamp_us, marker, payload_type, SmolRTSP_NalHeader_clock_rate_kHz(h),
        fu_header, SmolRTSP_NalUnit_payload(fu));
}
