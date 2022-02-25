#include <smolrtsp/rtp_transport.h>

#include <smolrtsp/types/rtp.h>

#include <assert.h>
#include <stdlib.h>

#include <alloca.h>
#include <arpa/inet.h>

struct SmolRTSP_RtpTransport {
    uint16_t seq_num;
    uint8_t rtsp_stream_id;
    SmolRTSP_Transport transport;
};

static uint32_t
compute_timestamp(uint64_t timestamp_us, uint64_t clock_rate_kHz);

SmolRTSP_RtpTransport *
SmolRTSP_RtpTransport_new(SmolRTSP_Transport t, uint8_t rtsp_stream_id) {
    assert(t.self && t.vptr);

    SmolRTSP_RtpTransport *self = malloc(sizeof *self);
    assert(self);

    self->seq_num = 0;
    self->rtsp_stream_id = rtsp_stream_id;
    self->transport = t;

    return self;
}

static void SmolRTSP_RtpTransport_drop(VSelf) {
    VSELF(SmolRTSP_RtpTransport);
    assert(self);

    VCALL_SUPER(self->transport, SmolRTSP_Droppable, drop);

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_RtpTransport);

int SmolRTSP_RtpTransport_send_packet(
    SmolRTSP_RtpTransport *self, uint64_t timestamp_us, bool marker,
    uint8_t payload_ty, uint32_t clock_rate, U8Slice99 payload_header,
    U8Slice99 payload) {
    assert(self);

    const SmolRTSP_RtpHeader header = {
        .version = 2,
        .padding = false,
        .extension = false,
        .csrc_count = 0,
        .marker = marker,
        .payload_ty = payload_ty,
        .sequence_number = htons(self->seq_num),
        .timestamp = htobe32(compute_timestamp(timestamp_us, clock_rate)),
        .ssrc = htobe32(0 == self->rtsp_stream_id ? 0x280951d0 : 0x36b9e9b4),
        .csrc = NULL,
        .extension_profile = htons(0),
        .extension_payload_len = htons(0),
        .extension_payload = NULL,
    };

    const size_t rtp_header_size = SmolRTSP_RtpHeader_size(header);
    const U8Slice99 rtp_header = U8Slice99_new(
        SmolRTSP_RtpHeader_serialize(header, alloca(rtp_header_size)),
        rtp_header_size);

    const SmolRTSP_IoVecSlice bufs =
        (SmolRTSP_IoVecSlice)Slice99_typed_from_array((struct iovec[]){
            smolrtsp_slice_to_iovec(rtp_header),
            smolrtsp_slice_to_iovec(payload_header),
            smolrtsp_slice_to_iovec(payload),
        });

    const int ret = VCALL(self->transport, transmit, bufs);
    if (ret != -1) {
        self->seq_num++;
    }

    return ret;
}

static uint32_t
compute_timestamp(uint64_t timestamp_us, uint64_t clock_rate_kHz) {
    const uint64_t timestamp_rem_us = (timestamp_us % 1000),
                   timestamp_millisecs =
                       (timestamp_us - timestamp_rem_us) / 1000;

    const uint32_t timestamp =
        timestamp_millisecs * clock_rate_kHz +
        (uint32_t)(timestamp_rem_us * ((double)clock_rate_kHz / 1000.0));

    return timestamp;
}
