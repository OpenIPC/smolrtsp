#include <smolrtsp/nal_transport.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include <alloca.h>

#include <slice99.h>

static int send_fragmentized_nal_data(
    SmolRTSP_RtpTransport *t, uint64_t timestamp_us, size_t max_packet_size,
    SmolRTSP_NalUnit nal_unit);
static int send_fu(
    SmolRTSP_RtpTransport *t, uint64_t timestamp_us, SmolRTSP_NalUnit fu,
    bool is_first_fragment, bool is_last_fragment);

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

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(self->transport);

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_NalTransport);

// TODO: justify these magic numbers.

#define MAX_H264_PACKET_SIZE 1200
#define MAX_H265_PACKET_SIZE 4096

int SmolRTSP_NalTransport_send_packet(
    SmolRTSP_NalTransport *self, uint64_t timestamp_us,
    SmolRTSP_NalUnit nal_unit) {
    assert(self);

    const size_t max_packet_size =
        MATCHES(nal_unit.header, SmolRTSP_NalHeader_H264)
            ? MAX_H264_PACKET_SIZE
            : MAX_H265_PACKET_SIZE;

    if (nal_unit.payload.len < max_packet_size) {
        const bool marker =
            SmolRTSP_NalHeader_is_coded_slice_idr(nal_unit.header) ||
            SmolRTSP_NalHeader_is_coded_slice_non_idr(nal_unit.header);

        const size_t header_buf_size = SmolRTSP_NalHeader_size(nal_unit.header);
        uint8_t *header_buf = alloca(header_buf_size);
        SmolRTSP_NalHeader_serialize(nal_unit.header, header_buf);

        return SmolRTSP_RtpTransport_send_packet(
            self->transport, timestamp_us, marker,
            U8Slice99_new(header_buf, header_buf_size), nal_unit.payload);
    }

    return send_fragmentized_nal_data(
        self->transport, timestamp_us, max_packet_size, nal_unit);
}

// See <https://tools.ietf.org/html/rfc6184#section-5.8> (H.264),
// <https://tools.ietf.org/html/rfc7798#section-4.4.3> (H.265).
static int send_fragmentized_nal_data(
    SmolRTSP_RtpTransport *t, uint64_t timestamp_us, size_t max_packet_size,
    SmolRTSP_NalUnit nal_unit) {
    const size_t rem = nal_unit.payload.len % max_packet_size,
                 packets_count = (nal_unit.payload.len - rem) / max_packet_size;

    for (size_t packet_idx = 0; packet_idx < packets_count; packet_idx++) {
        const bool is_first_fragment = 0 == packet_idx,
                   is_last_fragment =
                       0 == rem && (packets_count - 1 == packet_idx);

        const U8Slice99 fu_data = U8Slice99_sub(
            nal_unit.payload, packet_idx * max_packet_size,
            is_last_fragment ? nal_unit.payload.len
                             : (packet_idx + 1) * max_packet_size);
        const SmolRTSP_NalUnit fu = {nal_unit.header, fu_data};

        if (send_fu(t, timestamp_us, fu, is_first_fragment, is_last_fragment) ==
            -1) {
            return -1;
        }
    }

    if (rem != 0) {
        const U8Slice99 fu_data = U8Slice99_advance(
            nal_unit.payload, packets_count * max_packet_size);
        const SmolRTSP_NalUnit fu = {nal_unit.header, fu_data};
        const bool is_first_fragment = 0 == packets_count,
                   is_last_fragment = true;

        if (send_fu(t, timestamp_us, fu, is_first_fragment, is_last_fragment) ==
            -1) {
            return -1;
        }
    }

    return 0;
}

static int send_fu(
    SmolRTSP_RtpTransport *t, uint64_t timestamp_us, SmolRTSP_NalUnit fu,
    bool is_first_fragment, bool is_last_fragment) {
    const size_t fu_header_size = SmolRTSP_NalHeader_fu_size(fu.header);
    U8Slice99 fu_header = U8Slice99_new(alloca(fu_header_size), fu_header_size);

    SmolRTSP_NalHeader_write_fu_header(
        fu.header, fu_header.ptr, is_first_fragment, is_last_fragment);

    const bool marker = is_last_fragment;

    return SmolRTSP_RtpTransport_send_packet(
        t, timestamp_us, marker, fu_header, fu.payload);
}
