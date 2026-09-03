#include <smolrtsp/rtp_transport.h>
#include <smolrtsp/transport.h>

#include <greatest.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PAYLOAD_HEADER "Hdr"
#define PAYLOAD_BODY   "abcdefghij"

TEST accessors_initial_state(void) {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds));

    srand(1);
    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);
    SmolRTSP_RtpTransport *rtp =
        SmolRTSP_RtpTransport_new(udp, /*payload_ty=*/96, /*clock_rate=*/90000);
    ASSERT(rtp != NULL);

    /* SSRC is randomly generated; we cannot assert a specific value, only
     * that consecutive reads are stable. */
    const uint32_t ssrc0 = SmolRTSP_RtpTransport_ssrc(rtp);
    ASSERT_EQ_FMT(ssrc0, SmolRTSP_RtpTransport_ssrc(rtp), "%u");

    /* Fresh transport: no packets, no octets. */
    ASSERT_EQ_FMT((uint32_t)0, SmolRTSP_RtpTransport_pkt_count(rtp), "%u");
    ASSERT_EQ_FMT((uint32_t)0, SmolRTSP_RtpTransport_octet_count(rtp), "%u");

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(rtp);
    close(fds[1]);
    PASS();
}

TEST counters_advance_per_packet(void) {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds));

    srand(42);
    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);
    SmolRTSP_RtpTransport *rtp =
        SmolRTSP_RtpTransport_new(udp, /*payload_ty=*/96, /*clock_rate=*/90000);
    ASSERT(rtp != NULL);

    const uint32_t ssrc_before = SmolRTSP_RtpTransport_ssrc(rtp);

    U8Slice99 hdr =
        U8Slice99_new((uint8_t *)PAYLOAD_HEADER, sizeof(PAYLOAD_HEADER) - 1);
    U8Slice99 body =
        U8Slice99_new((uint8_t *)PAYLOAD_BODY, sizeof(PAYLOAD_BODY) - 1);
    const size_t pl_octets = hdr.len + body.len;

    const int r1 = SmolRTSP_RtpTransport_send_packet(
        rtp, SmolRTSP_RtpTimestamp_Raw(1234),
        /*marker=*/false, hdr, body);
    ASSERT_EQ(0, r1);
    ASSERT_EQ_FMT((uint32_t)1, SmolRTSP_RtpTransport_pkt_count(rtp), "%u");
    ASSERT_EQ_FMT(
        (uint32_t)pl_octets, SmolRTSP_RtpTransport_octet_count(rtp), "%u");

    /* Drain the receive side so the next send doesn't block. */
    char drain[256];
    (void)read(fds[1], drain, sizeof(drain));

    const int r2 = SmolRTSP_RtpTransport_send_packet(
        rtp, SmolRTSP_RtpTimestamp_Raw(2345),
        /*marker=*/true, hdr, body);
    ASSERT_EQ(0, r2);
    ASSERT_EQ_FMT((uint32_t)2, SmolRTSP_RtpTransport_pkt_count(rtp), "%u");
    ASSERT_EQ_FMT(
        (uint32_t)(pl_octets * 2), SmolRTSP_RtpTransport_octet_count(rtp),
        "%u");

    /* SSRC never changes for the lifetime of the transport. */
    ASSERT_EQ_FMT(ssrc_before, SmolRTSP_RtpTransport_ssrc(rtp), "%u");

    (void)read(fds[1], drain, sizeof(drain));

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(rtp);
    close(fds[1]);
    PASS();
}

TEST new_with_ssrc_uses_caller_value(void) {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds));

    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);
    SmolRTSP_RtpTransport *rtp = SmolRTSP_RtpTransport_new_with_ssrc(
        udp, /*payload_ty=*/96, /*clock_rate=*/90000,
        /*ssrc=*/0xdeadbeef);
    ASSERT(rtp != NULL);

    /* Caller-provided SSRC must be the one returned, not a rand() value. */
    ASSERT_EQ_FMT(
        (uint32_t)0xdeadbeef, SmolRTSP_RtpTransport_ssrc(rtp), "0x%08x");
    ASSERT_EQ_FMT((uint32_t)0, SmolRTSP_RtpTransport_pkt_count(rtp), "%u");
    ASSERT_EQ_FMT((uint32_t)0, SmolRTSP_RtpTransport_octet_count(rtp), "%u");

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(rtp);
    close(fds[1]);
    PASS();
}

/* Send one SysClockUs-stamped packet and report the RTP timestamp it put on
 * the wire, so the microseconds -> ticks conversion can be checked directly.
 * The timestamp base is pinned to zero: the scaling is what these vectors are
 * about, and the constructors that draw a random one would move every one of
 * them. */
static uint32_t sysclock_ts(uint32_t clock_rate, uint64_t time_us) {
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) != 0) {
        return UINT32_MAX;
    }

    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);
    SmolRTSP_RtpTransport *rtp = SmolRTSP_RtpTransport_new_with_ssrc_ts_base(
        udp, /*payload_ty=*/96, clock_rate, /*ssrc=*/1, /*ts_base_us=*/0);

    U8Slice99 hdr =
        U8Slice99_new((uint8_t *)PAYLOAD_HEADER, sizeof(PAYLOAD_HEADER) - 1);
    U8Slice99 body =
        U8Slice99_new((uint8_t *)PAYLOAD_BODY, sizeof(PAYLOAD_BODY) - 1);

    const int sent = SmolRTSP_RtpTransport_send_packet(
        rtp, SmolRTSP_RtpTimestamp_SysClockUs(time_us),
        /*marker=*/false, hdr, body);

    /* last_rtp_ts only advances on a successful send, so reporting it after a
     * failure would assert against a stale 0 and read as a scaling bug. There
     * is deliberately no drain read() here either: one packet per socketpair,
     * and both ends are closed below, so there is nothing to drain — a blind
     * read on a socket that never received anything would hang the suite
     * rather than fail it. */
    const uint32_t ts =
        sent == -1 ? UINT32_MAX : SmolRTSP_RtpTransport_last_rtp_ts(rtp);

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(rtp);
    close(fds[1]);

    return ts;
}

TEST sysclock_scales_exactly(void) {
    /* Whole-kHz rates always worked, and must not move. */
    ASSERT_EQ_FMT((uint32_t)90000, sysclock_ts(90000, 1000000), "%u");
    ASSERT_EQ_FMT((uint32_t)48000, sysclock_ts(48000, 1000000), "%u");
    ASSERT_EQ_FMT((uint32_t)8000, sysclock_ts(8000, 1000000), "%u");
    ASSERT_EQ_FMT((uint32_t)45000, sysclock_ts(90000, 500000), "%u");

    /* Rates that are not. 44100 used to truncate to 44000, losing 100 ticks
     * -- roughly 2.3 ms -- every second, without bound. */
    ASSERT_EQ_FMT((uint32_t)44100, sysclock_ts(44100, 1000000), "%u");
    ASSERT_EQ_FMT((uint32_t)441000, sysclock_ts(44100, 10000000), "%u");
    ASSERT_EQ_FMT((uint32_t)66150, sysclock_ts(44100, 1500000), "%u");
    ASSERT_EQ_FMT((uint32_t)22050, sysclock_ts(22050, 1000000), "%u");
    ASSERT_EQ_FMT((uint32_t)11025, sysclock_ts(11025, 1000000), "%u");

    /* Sub-kHz rates collapsed to zero outright. */
    ASSERT_EQ_FMT((uint32_t)900, sysclock_ts(900, 1000000), "%u");

    /* Sub-second remainders are floored, not discarded. */
    ASSERT_EQ_FMT((uint32_t)44, sysclock_ts(44100, 1000), "%u");
    ASSERT_EQ_FMT((uint32_t)90, sysclock_ts(90000, 1000), "%u");

    PASS();
}

TEST ts_base_shifts_the_media_clock(void) {
    /* The base is added in the microsecond domain, so one base shifts two
     * different clock rates by the same amount of TIME — which is what keeps
     * the audio and video of one session aligned for a receiver that has
     * only the timestamps to align them with. */
    ASSERT_EQ_FMT(
        (uint32_t)(90000 + 45000),
        smolrtsp_rtp_ts_from_sys_clock_us(1000000, 90000, 500000), "%u");
    ASSERT_EQ_FMT(
        (uint32_t)(8000 + 4000),
        smolrtsp_rtp_ts_from_sys_clock_us(1000000, 8000, 500000), "%u");

    /* Zero base is the plain conversion. */
    ASSERT_EQ_FMT(
        (uint32_t)90000, smolrtsp_rtp_ts_from_sys_clock_us(1000000, 90000, 0),
        "%u");

    /* Modular by definition: a base near the wrap does not saturate. */
    ASSERT_EQ_FMT(
        (uint32_t)((uint32_t)(477218588ull * 90000ull) + 90000u),
        smolrtsp_rtp_ts_from_sys_clock_us(1000000, 90000, 477218588000000ull),
        "%u");
    PASS();
}

TEST ts_base_is_random_by_default(void) {
    /* Two transports built the same way must not share an origin, or the
     * randomness RFC 3550 §5.1 asks for is not there at all. Ten draws: a
     * collision of two 62-bit bases is not something to retry over, but a
     * constant base would fail every pair. */
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) != 0) {
        FAIL();
    }
    uint32_t first = 0;
    bool differs = false;
    for (int i = 0; i < 10; i++) {
        SmolRTSP_RtpTransport *rtp = SmolRTSP_RtpTransport_new_with_ssrc(
            smolrtsp_transport_udp(fds[0]), /*payload_ty=*/96,
            /*clock_rate=*/90000, /*ssrc=*/1);
        const uint32_t ts =
            SmolRTSP_RtpTransport_ts_from_sys_clock_us(rtp, 1000000);
        if (i == 0) {
            first = ts;
        } else if (ts != first) {
            differs = true;
        }
        VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(rtp);
    }
    close(fds[1]);
    ASSERT(differs);
    PASS();
}

TEST ts_base_leaves_raw_timestamps_alone(void) {
    /* A Raw timestamp is the value the caller wants on the wire. */
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) != 0) {
        FAIL();
    }
    SmolRTSP_RtpTransport *rtp = SmolRTSP_RtpTransport_new_with_ssrc_ts_base(
        smolrtsp_transport_udp(fds[0]), /*payload_ty=*/96,
        /*clock_rate=*/90000, /*ssrc=*/1, /*ts_base_us=*/123456789);
    U8Slice99 hdr =
        U8Slice99_new((uint8_t *)PAYLOAD_HEADER, sizeof(PAYLOAD_HEADER) - 1);
    U8Slice99 body =
        U8Slice99_new((uint8_t *)PAYLOAD_BODY, sizeof(PAYLOAD_BODY) - 1);
    const int sent = SmolRTSP_RtpTransport_send_packet(
        rtp, SmolRTSP_RtpTimestamp_Raw(0xDEADBEEF), /*marker=*/false, hdr,
        body);
    const uint32_t ts = sent == -1 ? 0 : SmolRTSP_RtpTransport_last_rtp_ts(rtp);
    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(rtp);
    close(fds[1]);
    ASSERT_EQ_FMT((uint32_t)0xDEADBEEF, ts, "%u");
    PASS();
}

SUITE(rtp_transport) {
    RUN_TEST(accessors_initial_state);
    RUN_TEST(counters_advance_per_packet);
    RUN_TEST(new_with_ssrc_uses_caller_value);
    RUN_TEST(sysclock_scales_exactly);
    RUN_TEST(ts_base_shifts_the_media_clock);
    RUN_TEST(ts_base_is_random_by_default);
    RUN_TEST(ts_base_leaves_raw_timestamps_alone);
}
