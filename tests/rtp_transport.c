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

SUITE(rtp_transport) {
    RUN_TEST(accessors_initial_state);
    RUN_TEST(counters_advance_per_packet);
}
