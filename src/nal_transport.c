#include <smolrtsp/nal_transport.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include <alloca.h>

#include <slice99.h>

static int send_fragmentized_nal_data(
    SmolRTSP_RtpTransport *t, SmolRTSP_RtpTimestamp ts, size_t max_packet_size,
    SmolRTSP_NalUnit nalu);
static int send_fu(
    SmolRTSP_RtpTransport *t, SmolRTSP_RtpTimestamp ts, SmolRTSP_NalUnit fu,
    bool is_first_fragment, bool is_last_fragment);

SmolRTSP_NalTransportConfig SmolRTSP_NalTransportConfig_default(void) {
    return (SmolRTSP_NalTransportConfig){
        .max_h264_nalu_size = SMOLRTSP_MAX_H264_NALU_SIZE,
        .max_h265_nalu_size = SMOLRTSP_MAX_H265_NALU_SIZE,
    };
}

struct SmolRTSP_NalTransport {
    SmolRTSP_RtpTransport *transport;
    SmolRTSP_NalTransportConfig config;
};

SmolRTSP_NalTransport *SmolRTSP_NalTransport_new(SmolRTSP_RtpTransport *t) {
    assert(t);

    return SmolRTSP_NalTransport_new_with_config(
        t, SmolRTSP_NalTransportConfig_default());
}

SmolRTSP_NalTransport *SmolRTSP_NalTransport_new_with_config(
    SmolRTSP_RtpTransport *t, SmolRTSP_NalTransportConfig config) {
    assert(t);

    SmolRTSP_NalTransport *self = malloc(sizeof *self);
    assert(self);

    self->transport = t;
    self->config = config;

    return self;
}

static void SmolRTSP_NalTransport_drop(VSelf) {
    VSELF(SmolRTSP_NalTransport);
    assert(self);

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(self->transport);

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_NalTransport);

bool SmolRTSP_NalTransport_is_full(SmolRTSP_NalTransport *self) {
    return SmolRTSP_RtpTransport_is_full(self->transport);
}

int SmolRTSP_NalTransport_send_packet(
    SmolRTSP_NalTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_NalUnit nalu) {
    assert(self);

    const size_t max_packet_size = MATCHES(nalu.header, SmolRTSP_NalHeader_H264)
                                       ? self->config.max_h264_nalu_size
                                       : self->config.max_h265_nalu_size,
                 nalu_size =
                     SmolRTSP_NalHeader_size(nalu.header) + nalu.payload.len;

    if (nalu_size < max_packet_size) {
        const bool marker =
            SmolRTSP_NalHeader_is_coded_slice_idr(nalu.header) ||
            SmolRTSP_NalHeader_is_coded_slice_non_idr(nalu.header);

        const size_t header_buf_size = SmolRTSP_NalHeader_size(nalu.header);
        uint8_t *header_buf = alloca(header_buf_size);
        SmolRTSP_NalHeader_serialize(nalu.header, header_buf);

        return SmolRTSP_RtpTransport_send_packet(
            self->transport, ts, marker,
            U8Slice99_new(header_buf, header_buf_size), nalu.payload);
    }

    return send_fragmentized_nal_data(
        self->transport, ts, max_packet_size, nalu);
}

// See <https://tools.ietf.org/html/rfc6184#section-5.8> (H.264),
// <https://tools.ietf.org/html/rfc7798#section-4.4.3> (H.265).
static int send_fragmentized_nal_data(
    SmolRTSP_RtpTransport *t, SmolRTSP_RtpTimestamp ts, size_t max_packet_size,
    SmolRTSP_NalUnit nalu) {
    const size_t rem = nalu.payload.len % max_packet_size,
                 packets_count = (nalu.payload.len - rem) / max_packet_size;

    for (size_t packet_idx = 0; packet_idx < packets_count; packet_idx++) {
        const bool is_first_fragment = 0 == packet_idx,
                   is_last_fragment =
                       0 == rem && (packets_count - 1 == packet_idx);

        const U8Slice99 fu_data = U8Slice99_sub(
            nalu.payload, packet_idx * max_packet_size,
            is_last_fragment ? nalu.payload.len
                             : (packet_idx + 1) * max_packet_size);
        const SmolRTSP_NalUnit fu = {nalu.header, fu_data};

        if (send_fu(t, ts, fu, is_first_fragment, is_last_fragment) == -1) {
            return -1;
        }
    }

    if (rem != 0) {
        const U8Slice99 fu_data =
            U8Slice99_advance(nalu.payload, packets_count * max_packet_size);
        const SmolRTSP_NalUnit fu = {nalu.header, fu_data};
        const bool is_first_fragment = 0 == packets_count,
                   is_last_fragment = true;

        if (send_fu(t, ts, fu, is_first_fragment, is_last_fragment) == -1) {
            return -1;
        }
    }

    return 0;
}

static int send_fu(
    SmolRTSP_RtpTransport *t, SmolRTSP_RtpTimestamp ts, SmolRTSP_NalUnit fu,
    bool is_first_fragment, bool is_last_fragment) {
    const size_t fu_header_size = SmolRTSP_NalHeader_fu_size(fu.header);
    U8Slice99 fu_header = U8Slice99_new(alloca(fu_header_size), fu_header_size);

    SmolRTSP_NalHeader_write_fu_header(
        fu.header, fu_header.ptr, is_first_fragment, is_last_fragment);

    const bool marker = is_last_fragment;

    return SmolRTSP_RtpTransport_send_packet(
        t, ts, marker, fu_header, fu.payload);
}
