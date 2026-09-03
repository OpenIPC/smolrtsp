#include <smolrtsp/rtp_transport.h>

#include <smolrtsp/types/rtp.h>

#include <assert.h>
#include <stdlib.h>

#include <alloca.h>
#include <arpa/inet.h>

struct SmolRTSP_RtpTransport {
    uint16_t seq_num;
    uint32_t ssrc;
    uint32_t pkt_count;
    uint32_t octet_count;
    uint32_t last_rtp_ts;
    uint8_t payload_ty;
    uint32_t clock_rate;
    uint64_t ts_base_us;
    SmolRTSP_Transport transport;
};

static uint32_t compute_timestamp(
    SmolRTSP_RtpTimestamp ts, uint32_t clock_rate, uint64_t ts_base_us);

/* RFC 3550 §5.1: "the initial value of the timestamp SHOULD be random".
 *
 * Assembled from as many draws as the platform's RAND_MAX needs rather than
 * from a fixed two: RAND_MAX is only guaranteed to reach 32767, and two
 * draws of that width leave two thirds of the base's range unreachable —
 * a base that cannot span the timestamp space leaves the origin partly
 * known, which is the whole thing this is for. Six days of microseconds is
 * what a 32-bit timestamp covers at 8 kHz, so the width has to be built up
 * to 64 bits, not assumed.
 *
 * rand() is what a C99 library has, and it is what the SSRC above already
 * uses; a transport constructor is not the place to open /dev/urandom on a
 * caller's behalf. What it is worth therefore depends on how the caller
 * seeded it: an attacker who can reproduce the sequence recovers the base,
 * and the SSRC drawn beside it is on the wire to check the guess against.
 * A caller who needs better than that — because the base is hiding
 * something, as it is for a camera whose media clock is its uptime — passes
 * its own to SmolRTSP_RtpTransport_new_with_ssrc_ts_base(). */
static uint64_t random_ts_base_us(void) {
    /* RAND_MAX is one less than a power of two on every implementation in
     * practice; its width is what a draw actually carries. */
    unsigned per_draw = 0;
    for (int m = RAND_MAX; m > 0; m >>= 1) {
        per_draw++;
    }

    uint64_t base = 0;
    for (unsigned bits = 0; bits < 64; bits += per_draw) {
        base = (base << per_draw) ^ (uint64_t)(unsigned)rand();
    }
    return base;
}

SmolRTSP_RtpTransport *SmolRTSP_RtpTransport_new(
    SmolRTSP_Transport t, uint8_t payload_ty, uint32_t clock_rate) {
    return SmolRTSP_RtpTransport_new_with_ssrc(
        t, payload_ty, clock_rate, (uint32_t)rand());
}

SmolRTSP_RtpTransport *SmolRTSP_RtpTransport_new_with_ssrc(
    SmolRTSP_Transport t, uint8_t payload_ty, uint32_t clock_rate,
    uint32_t ssrc) {
    return SmolRTSP_RtpTransport_new_with_ssrc_ts_base(
        t, payload_ty, clock_rate, ssrc, random_ts_base_us());
}

SmolRTSP_RtpTransport *SmolRTSP_RtpTransport_new_with_ssrc_ts_base(
    SmolRTSP_Transport t, uint8_t payload_ty, uint32_t clock_rate,
    uint32_t ssrc, uint64_t ts_base_us) {
    assert(t.self && t.vptr);

    SmolRTSP_RtpTransport *self = malloc(sizeof *self);
    assert(self);

    self->seq_num = 0;
    self->ssrc = ssrc;
    self->pkt_count = 0;
    self->octet_count = 0;
    self->last_rtp_ts = 0;
    self->payload_ty = payload_ty;
    self->clock_rate = clock_rate;
    self->ts_base_us = ts_base_us;
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

    const uint32_t rtp_ts =
        compute_timestamp(ts, self->clock_rate, self->ts_base_us);

    const SmolRTSP_RtpHeader header = {
        .version = 2,
        .padding = false,
        .extension = false,
        .csrc_count = 0,
        .marker = marker,
        .payload_ty = self->payload_ty,
        .sequence_number = htons(self->seq_num),
        .timestamp = htobe32(rtp_ts),
        /* SSRC is the only multi-byte RTP-header field that was
         * passed in host order — every other (sequence, timestamp,
         * extension_*) is htobe32/htons'd in this same struct.
         * Without this swap, on little-endian hosts the wire bytes
         * are the byte-reversal of the SSRC, so receivers can't
         * correlate the RTP stream with the htonl-ed SSRC in our
         * RTCP packets (SR/RR/SDES/BYE). RFC 3550 §5.1 requires
         * network byte order for all multi-byte RTP fields. */
        .ssrc = htonl(self->ssrc),
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
        self->pkt_count++;
        /* RFC 3550 §6.4.1: octet count covers payload only (no RTP header
         * or padding). The codec-specific `payload_header` is part of the
         * payload from the receiver's point of view. */
        self->octet_count += (uint32_t)(payload_header.len + payload.len);
        self->last_rtp_ts = rtp_ts;
    }

    return ret;
}

static uint32_t compute_timestamp(
    SmolRTSP_RtpTimestamp ts, uint32_t clock_rate, uint64_t ts_base_us) {
    match(ts) {
        of(SmolRTSP_RtpTimestamp_Raw, raw_ts) {
            /* Already a wire value; shifting it would corrupt a caller that
             * knows exactly what it wants to send. */
            return *raw_ts;
        }
        of(SmolRTSP_RtpTimestamp_SysClockUs, time_us) {
            /* Scale exactly: ticks = time_us * clock_rate / 10^6.
             *
             * Pre-dividing the clock rate to whole kHz, as this used to do,
             * is lossy for any rate that is not a multiple of 1000 — 44100
             * became 44000, drifting ~2.3 ms per second without bound, and
             * anything below 1000 Hz collapsed to zero.
             *
             * Splitting on seconds keeps every intermediate within 64 bits:
             * `sec * clock_rate` cannot realistically overflow, and
             * `us_rem < 10^6` bounds the second term far below it. Because
             * `sec * clock_rate` is an integer, the two truncations compose
             * into the single floor the formula wants. Narrowing to uint32_t
             * preserves the modular wrap RTP timestamps are defined to have. */
            return smolrtsp_rtp_ts_from_sys_clock_us(
                *time_us, clock_rate, ts_base_us);
        }
    }

    return 0;
}

uint32_t smolrtsp_rtp_ts_from_sys_clock_us(
    uint64_t time_us, uint32_t clock_rate, uint64_t ts_base_us) {
    /* The base shifts the clock's origin, and is added in the microsecond
     * domain so that streams sharing a base stay aligned across different
     * clock rates. Both it and the sum wrap by design — an RTP timestamp is
     * modular arithmetic (RFC 3550 §5.1). */
    const uint64_t t = time_us + ts_base_us;
    const uint64_t sec = t / 1000000, us_rem = t % 1000000;
    return (uint32_t)(sec * clock_rate + us_rem * clock_rate / 1000000);
}

uint32_t SmolRTSP_RtpTransport_ts_from_sys_clock_us(
    const SmolRTSP_RtpTransport *self, uint64_t time_us) {
    assert(self);
    return smolrtsp_rtp_ts_from_sys_clock_us(
        time_us, self->clock_rate, self->ts_base_us);
}

bool SmolRTSP_RtpTransport_is_full(SmolRTSP_RtpTransport *self) {
    return VCALL(self->transport, is_full);
}

uint32_t SmolRTSP_RtpTransport_ssrc(SmolRTSP_RtpTransport *self) {
    assert(self);
    return self->ssrc;
}

uint32_t SmolRTSP_RtpTransport_pkt_count(SmolRTSP_RtpTransport *self) {
    assert(self);
    return self->pkt_count;
}

uint32_t SmolRTSP_RtpTransport_octet_count(SmolRTSP_RtpTransport *self) {
    assert(self);
    return self->octet_count;
}

uint32_t SmolRTSP_RtpTransport_last_rtp_ts(SmolRTSP_RtpTransport *self) {
    assert(self);
    return self->last_rtp_ts;
}
