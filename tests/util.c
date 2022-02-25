#include <smolrtsp/util.h>

#include <greatest.h>

TEST parse_lower_transport(void) {
    SmolRTSP_LowerTransport lower_transport;

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport,
            CharSlice99_from_str("RTP/AVP/UDP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_UDP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport,
            CharSlice99_from_str("RTP/AVP/TCP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_TCP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport,
            CharSlice99_from_str("RTP/AVP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_UDP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport,
            CharSlice99_from_str("RTP/blah;unicast;client_port=3056-3057")),
        -1);

    PASS();
}

TEST parse_client_port(void) {
    int rtp_port, rtcp_port;

    ASSERT_EQ(
        SmolRTSP_parse_client_port(
            &rtp_port, &rtcp_port,
            CharSlice99_from_str("RTP/AVP/UDP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(rtp_port, 3056);
    ASSERT_EQ(rtcp_port, 3057);

    ASSERT_EQ(
        SmolRTSP_parse_client_port(
            &rtp_port, &rtcp_port,
            CharSlice99_from_str(
                "RTP/AVP/"
                "UDP;unicast;client_port=3058-3059;server_port=5002-5003")),
        0);
    ASSERT_EQ(rtp_port, 3058);
    ASSERT_EQ(rtcp_port, 3059);

    PASS();
}

TEST parse_interleaved_chn_id(void) {
    int rtp_chn_id, rtcp_chn_id;

    ASSERT_EQ(
        SmolRTSP_parse_interleaved_chn_id(
            &rtp_chn_id, &rtcp_chn_id,
            CharSlice99_from_str("RTP/AVP/UDP;unicast;interleaved=204")),
        0);
    ASSERT_EQ(rtp_chn_id, 204);
    ASSERT_EQ(rtcp_chn_id, -1);

    ASSERT_EQ(
        SmolRTSP_parse_interleaved_chn_id(
            &rtp_chn_id, &rtcp_chn_id,
            CharSlice99_from_str(
                "RTP/AVP/"
                "UDP;unicast;interleaved=3058-3059;server_port=5002-5003")),
        0);
    ASSERT_EQ(rtp_chn_id, 3058);
    ASSERT_EQ(rtcp_chn_id, 3059);

    PASS();
}

TEST interleaved_header(void) {
    uint8_t channel_id = 123;
    uint16_t payload_len = 54321;

    const uint32_t binary =
        smolrtsp_interleaved_header(channel_id, payload_len);

    ASSERT_EQ(0xd4317b24, binary);

    PASS();
}

TEST parse_interleaved_header(void) {
    uint8_t channel_id = 0;
    uint16_t payload_len = 0;

    smolrtsp_parse_interleaved_header(0xd4317b24, &channel_id, &payload_len);

    ASSERT_EQ(123, channel_id);
    ASSERT_EQ(54321, payload_len);

    PASS();
}

SUITE(util) {
    RUN_TEST(parse_lower_transport);
    RUN_TEST(parse_client_port);
    RUN_TEST(parse_interleaved_chn_id);
    RUN_TEST(interleaved_header);
    RUN_TEST(parse_interleaved_header);
}
