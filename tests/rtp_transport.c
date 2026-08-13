#include <smolrtsp/rtp_transport.h>
#include <smolrtsp/transport.h>

#include <greatest.h>

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
 * the wire, so the microseconds -> ticks conversion can be checked directly. */
static uint32_t sysclock_ts(uint32_t clock_rate, uint64_t time_us) {
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) != 0) {
        return UINT32_MAX;
    }

    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);
    SmolRTSP_RtpTransport *rtp = SmolRTSP_RtpTransport_new_with_ssrc(
        udp, /*payload_ty=*/96, clock_rate, /*ssrc=*/1);

    U8Slice99 hdr =
        U8Slice99_new((uint8_t *)PAYLOAD_HEADER, sizeof(PAYLOAD_HEADER) - 1);
    U8Slice99 body =
        U8Slice99_new((uint8_t *)PAYLOAD_BODY, sizeof(PAYLOAD_BODY) - 1);

    (void)SmolRTSP_RtpTransport_send_packet(
        rtp, SmolRTSP_RtpTimestamp_SysClockUs(time_us),
        /*marker=*/false, hdr, body);

    const uint32_t ts = SmolRTSP_RtpTransport_last_rtp_ts(rtp);

    char drain[256];
    (void)read(fds[1], drain, sizeof(drain));

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

SUITE(rtp_transport) {
    RUN_TEST(accessors_initial_state);
    RUN_TEST(counters_advance_per_packet);
    RUN_TEST(new_with_ssrc_uses_caller_value);
    RUN_TEST(sysclock_scales_exactly);
}
