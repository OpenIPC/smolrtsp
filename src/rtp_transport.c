#include <smolrtsp/rtp_transport.h>

#include <smolrtsp/types/rtp.h>

#include <assert.h>
#include <stdlib.h>

#include <alloca.h>
#include <arpa/inet.h>

struct SmolRTSP_RtpTransport {
    uint16_t seq_num;
    uint32_t ssrc;
    uint8_t payload_ty;
    uint32_t clock_rate;
    SmolRTSP_Transport transport;
};

static uint32_t
compute_timestamp(SmolRTSP_RtpTimestamp ts, uint32_t clock_rate);

SmolRTSP_RtpTransport *SmolRTSP_RtpTransport_new(
    SmolRTSP_Transport t, uint8_t payload_ty, uint32_t clock_rate) {
    assert(t.self && t.vptr);

    SmolRTSP_RtpTransport *self = malloc(sizeof *self);
    assert(self);

    self->seq_num = 0;
    self->ssrc = (uint32_t)rand();
    self->payload_ty = payload_ty;
    self->clock_rate = clock_rate;
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
    SmolRTSP_RtpTransport *self, SmolRTSP_RtpTimestamp ts, bool marker,
    U8Slice99 payload_header, U8Slice99 payload) {
    assert(self);

    const SmolRTSP_RtpHeader header = {
        .version = 2,
        .padding = false,
        .extension = false,
        .csrc_count = 0,
        .marker = marker,
        .payload_ty = self->payload_ty,
        .sequence_number = htons(self->seq_num),
        .timestamp = htobe32(compute_timestamp(ts, self->clock_rate)),
        .ssrc = self->ssrc,
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
compute_timestamp(SmolRTSP_RtpTimestamp ts, uint32_t clock_rate) {
    match(ts) {
        of(SmolRTSP_RtpTimestamp_Raw, raw_ts) {
            return *raw_ts;
        }
        of(SmolRTSP_RtpTimestamp_SysClockUs, time_us) {
            const uint64_t us_rem = *time_us % 1000,
                           ms = (*time_us - us_rem) / 1000;
            uint32_t clock_rate_kHz = clock_rate / 1000;
            return ms * clock_rate_kHz +
                   (uint32_t)(us_rem * ((double)clock_rate_kHz / 1000.0));
        }
    }

    return 0;
}

bool SmolRTSP_RtpTransport_is_full(SmolRTSP_RtpTransport *self) {
    return VCALL(self->transport, is_full);
}
