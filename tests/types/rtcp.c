#include <smolrtsp/types/rtcp.h>

#include <arpa/inet.h>
#include <string.h>

#include <greatest.h>

TEST sr_size_is_28_for_no_report_blocks(void) {
    const SmolRTSP_RtcpSr sr = {0};
    ASSERT_EQ((size_t)28, SmolRTSP_RtcpSr_size(sr));
    ASSERT_EQ((size_t)SMOLRTSP_RTCP_SR_SIZE_BASE, SmolRTSP_RtcpSr_size(sr));
    PASS();
}

TEST sr_serialize_wire_format(void) {
    /*
     * Build an SR with known field values, then check every byte against the
     * RFC 3550 §6.4.1 wire format.
     */
    const SmolRTSP_RtcpSr sr = {
        .padding = false,
        .rc = 0,
        .ssrc = htonl(0xdeadbeef),
        .ntp_sec = htonl(0xe79c69b4u),  /* example NTP seconds */
        .ntp_frac = htonl(0x80000000u), /* exactly 0.5 s */
        .rtp_ts = htonl(0x12345678u),
        .pkt_count = htonl(42),
        .octet_count = htonl(1234567),
    };

    uint8_t buf[64] = {0};
    const uint8_t *ret = SmolRTSP_RtcpSr_serialize(sr, buf);
    ASSERT_EQ((const uint8_t *)buf, ret);

    /* Common header byte 0: V=2 (10), P=0, RC=0 → 0b10000000 = 0x80. */
    ASSERT_EQ_FMT((uint8_t)0x80, buf[0], "0x%02x");

    /* Payload type byte 1: SR = 200 (0xc8). */
    ASSERT_EQ_FMT((uint8_t)200, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)SMOLRTSP_RTCP_PT_SR, buf[1], "%u");

    /* Length bytes 2-3: 28 B / 4 - 1 = 6 → 0x0006 in network order. */
    ASSERT_EQ_FMT((uint8_t)0x00, buf[2], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x06, buf[3], "0x%02x");

    /* SSRC bytes 4-7: 0xdeadbeef in network order. */
    ASSERT_EQ_FMT((uint8_t)0xde, buf[4], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xad, buf[5], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xbe, buf[6], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xef, buf[7], "0x%02x");

    /* NTP seconds 8-11. */
    ASSERT_EQ_FMT((uint8_t)0xe7, buf[8], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x9c, buf[9], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x69, buf[10], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xb4, buf[11], "0x%02x");

    /* NTP fraction 12-15: 0x80000000 = 0.5 s. */
    ASSERT_EQ_FMT((uint8_t)0x80, buf[12], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[13], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[14], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[15], "0x%02x");

    /* RTP timestamp 16-19. */
    ASSERT_EQ_FMT((uint8_t)0x12, buf[16], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x34, buf[17], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x56, buf[18], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x78, buf[19], "0x%02x");

    /* Packet count 20-23: 42 = 0x0000002a. */
    ASSERT_EQ_FMT((uint8_t)0x00, buf[20], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[21], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[22], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x2a, buf[23], "0x%02x");

    /* Octet count 24-27: 1234567 = 0x0012d687. */
    ASSERT_EQ_FMT((uint8_t)0x00, buf[24], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x12, buf[25], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xd6, buf[26], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x87, buf[27], "0x%02x");

    /* Trailing bytes untouched. */
    for (size_t i = SMOLRTSP_RTCP_SR_SIZE_BASE; i < sizeof(buf); i++) {
        ASSERT_EQ_FMT((uint8_t)0, buf[i], "0x%02x");
    }

    PASS();
}

TEST sr_serialize_padding_flag(void) {
    SmolRTSP_RtcpSr sr = {0};
    sr.padding = true;

    uint8_t buf[SMOLRTSP_RTCP_SR_SIZE_BASE] = {0};
    const uint8_t *ret = SmolRTSP_RtcpSr_serialize(sr, buf);
    ASSERT_EQ((const uint8_t *)buf, ret);

    /* V=2, P=1, RC=0 → 0b10100000 = 0xa0. */
    ASSERT_EQ_FMT((uint8_t)0xa0, buf[0], "0x%02x");

    PASS();
}

SUITE(types_rtcp) {
    RUN_TEST(sr_size_is_28_for_no_report_blocks);
    RUN_TEST(sr_serialize_wire_format);
    RUN_TEST(sr_serialize_padding_flag);
}
