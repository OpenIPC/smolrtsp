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

TEST rr_size_is_8_for_no_report_blocks(void) {
    const SmolRTSP_RtcpRr rr = {0};
    ASSERT_EQ((size_t)8, SmolRTSP_RtcpRr_size(rr));
    ASSERT_EQ((size_t)SMOLRTSP_RTCP_RR_SIZE_BASE, SmolRTSP_RtcpRr_size(rr));
    PASS();
}

TEST rr_serialize_wire_format(void) {
    const SmolRTSP_RtcpRr rr = {
        .padding = false,
        .rc = 0,
        .ssrc = htonl(0xcafebabe),
    };

    uint8_t buf[16] = {0};
    const uint8_t *ret = SmolRTSP_RtcpRr_serialize(rr, buf);
    ASSERT_EQ((const uint8_t *)buf, ret);

    /* V=2 P=0 RC=0 PT=201 length=1 (= 8/4 - 1). */
    ASSERT_EQ_FMT((uint8_t)0x80, buf[0], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)201, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)SMOLRTSP_RTCP_PT_RR, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[2], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x01, buf[3], "0x%02x");
    /* SSRC bytes 4-7. */
    ASSERT_EQ_FMT((uint8_t)0xca, buf[4], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xfe, buf[5], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xba, buf[6], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xbe, buf[7], "0x%02x");
    /* Bytes after the RR are untouched. */
    for (size_t i = 8; i < sizeof(buf); i++) {
        ASSERT_EQ_FMT((uint8_t)0, buf[i], "0x%02x");
    }

    PASS();
}

TEST sdes_cname_size_padding(void) {
    /* CNAME "ab" → raw size = 4 + 4 + 2 + 2 + 1 = 13, pad to 16. */
    SmolRTSP_RtcpSdesCname s = {.cname = "ab"};
    ASSERT_EQ((size_t)16, SmolRTSP_RtcpSdesCname_size(s));

    /* CNAME "abcdef" → raw = 4+4+2+6+1 = 17, pad to 20. */
    s.cname = "abcdef";
    ASSERT_EQ((size_t)20, SmolRTSP_RtcpSdesCname_size(s));

    /* CNAME "" → raw = 4+4+2+0+1 = 11, pad to 12. */
    s.cname = "";
    ASSERT_EQ((size_t)12, SmolRTSP_RtcpSdesCname_size(s));

    PASS();
}

TEST sdes_cname_serialize_wire_format(void) {
    const SmolRTSP_RtcpSdesCname s = {
        .padding = false,
        .ssrc = htonl(0xdeadbeef),
        .cname = "u@h",
    };
    /* Expected size: 4 + 4 + 2 + 3 + 1 = 14, padded to 16. */
    const size_t expected_size = 16;
    ASSERT_EQ(expected_size, SmolRTSP_RtcpSdesCname_size(s));

    uint8_t buf[32] = {0};
    const uint8_t *ret = SmolRTSP_RtcpSdesCname_serialize(s, buf);
    ASSERT_EQ((const uint8_t *)buf, ret);

    /* Common header: V=2 P=0 SC=1 → 0x81, PT=202, length = 16/4 - 1 = 3 */
    ASSERT_EQ_FMT((uint8_t)0x81, buf[0], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)SMOLRTSP_RTCP_PT_SDES, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[2], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x03, buf[3], "0x%02x");

    /* SSRC. */
    ASSERT_EQ_FMT((uint8_t)0xde, buf[4], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xad, buf[5], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xbe, buf[6], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xef, buf[7], "0x%02x");

    /* CNAME item: type 1, length 3, then "u@h". */
    ASSERT_EQ_FMT((uint8_t)SMOLRTSP_RTCP_SDES_CNAME, buf[8], "%u");
    ASSERT_EQ_FMT((uint8_t)3, buf[9], "%u");
    ASSERT_EQ_FMT((uint8_t)'u', buf[10], "%u");
    ASSERT_EQ_FMT((uint8_t)'@', buf[11], "%u");
    ASSERT_EQ_FMT((uint8_t)'h', buf[12], "%u");

    /* Item terminator (type 0) + 0-padding to 16 B. */
    ASSERT_EQ_FMT((uint8_t)0, buf[13], "%u");
    ASSERT_EQ_FMT((uint8_t)0, buf[14], "%u");
    ASSERT_EQ_FMT((uint8_t)0, buf[15], "%u");

    /* Bytes after the SDES are untouched. */
    for (size_t i = 16; i < sizeof(buf); i++) {
        ASSERT_EQ_FMT((uint8_t)0, buf[i], "0x%02x");
    }

    PASS();
}

TEST bye_size_no_reason(void) {
    const SmolRTSP_RtcpBye b = {.ssrc = htonl(0x1234), .reason = NULL};
    ASSERT_EQ((size_t)8, SmolRTSP_RtcpBye_size(b));

    const SmolRTSP_RtcpBye b_empty = {.ssrc = htonl(0x1234), .reason = ""};
    ASSERT_EQ((size_t)8, SmolRTSP_RtcpBye_size(b_empty));
    PASS();
}

TEST bye_size_with_reason(void) {
    /* "bye!" (4 chars) → raw = 4 + 4 + 1 + 4 = 13, pad to 16. */
    const SmolRTSP_RtcpBye b = {.reason = "bye!"};
    ASSERT_EQ((size_t)16, SmolRTSP_RtcpBye_size(b));
    PASS();
}

TEST bye_serialize_no_reason(void) {
    const SmolRTSP_RtcpBye b = {
        .padding = false,
        .ssrc = htonl(0xfeedface),
        .reason = NULL,
    };
    uint8_t buf[16] = {0};
    const uint8_t *ret = SmolRTSP_RtcpBye_serialize(b, buf);
    ASSERT_EQ((const uint8_t *)buf, ret);

    /* V=2 P=0 SC=1 → 0x81, PT=203, length = 8/4 - 1 = 1. */
    ASSERT_EQ_FMT((uint8_t)0x81, buf[0], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)SMOLRTSP_RTCP_PT_BYE, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[2], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0x01, buf[3], "0x%02x");
    /* SSRC */
    ASSERT_EQ_FMT((uint8_t)0xfe, buf[4], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xed, buf[5], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xfa, buf[6], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)0xce, buf[7], "0x%02x");
    /* Nothing after the SSRC. */
    for (size_t i = 8; i < sizeof(buf); i++) {
        ASSERT_EQ_FMT((uint8_t)0, buf[i], "0x%02x");
    }

    PASS();
}

TEST bye_serialize_with_reason(void) {
    const SmolRTSP_RtcpBye b = {
        .padding = false,
        .ssrc = htonl(0x01020304),
        .reason = "bye!",
    };
    /* Expected size 16: 4 header + 4 ssrc + 1 len + 4 reason + 3 pad. */
    ASSERT_EQ((size_t)16, SmolRTSP_RtcpBye_size(b));

    uint8_t buf[32] = {0};
    SmolRTSP_RtcpBye_serialize(b, buf);

    /* Header: SC=1, PT=203, length = 16/4 - 1 = 3. */
    ASSERT_EQ_FMT((uint8_t)0x81, buf[0], "0x%02x");
    ASSERT_EQ_FMT((uint8_t)SMOLRTSP_RTCP_PT_BYE, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)0x03, buf[3], "0x%02x");
    /* Reason length = 4, then "bye!" text. */
    ASSERT_EQ_FMT((uint8_t)4, buf[8], "%u");
    ASSERT_EQ_FMT((uint8_t)'b', buf[9], "%u");
    ASSERT_EQ_FMT((uint8_t)'y', buf[10], "%u");
    ASSERT_EQ_FMT((uint8_t)'e', buf[11], "%u");
    ASSERT_EQ_FMT((uint8_t)'!', buf[12], "%u");
    /* Padding bytes 13-15 zero. */
    ASSERT_EQ_FMT((uint8_t)0, buf[13], "%u");
    ASSERT_EQ_FMT((uint8_t)0, buf[14], "%u");
    ASSERT_EQ_FMT((uint8_t)0, buf[15], "%u");

    PASS();
}

SUITE(types_rtcp) {
    RUN_TEST(sr_size_is_28_for_no_report_blocks);
    RUN_TEST(sr_serialize_wire_format);
    RUN_TEST(sr_serialize_padding_flag);
    RUN_TEST(rr_size_is_8_for_no_report_blocks);
    RUN_TEST(rr_serialize_wire_format);
    RUN_TEST(sdes_cname_size_padding);
    RUN_TEST(sdes_cname_serialize_wire_format);
    RUN_TEST(bye_size_no_reason);
    RUN_TEST(bye_size_with_reason);
    RUN_TEST(bye_serialize_no_reason);
    RUN_TEST(bye_serialize_with_reason);
}
