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

TEST interleaved_data_as_u32(void) {
    const SmolRTSP_InterleavedDataHeader h = {
        .channel_id = 123,
        .payload_len = 54321,
    };

    const uint32_t binary = SmolRTSP_InterleavedDataHeader_as_u32(h);

    ASSERT_EQ(binary, 0xd4317b24);

    PASS();
}

TEST interleaved_data_from_u32(void) {
    const SmolRTSP_InterleavedDataHeader h =
        SmolRTSP_InterleavedDataHeader_from_u32(0xd4317b24);

    ASSERT_EQ(h.channel_id, 123);
    ASSERT_EQ(h.payload_len, 54321);

    PASS();
}

SUITE(util) {
    RUN_TEST(parse_lower_transport);
    RUN_TEST(parse_client_port);
    RUN_TEST(parse_interleaved_chn_id);
    RUN_TEST(interleaved_data_as_u32);
    RUN_TEST(interleaved_data_from_u32);
}
