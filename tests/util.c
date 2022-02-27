#include <smolrtsp/util.h>

#include <inttypes.h>
#include <stdint.h>

#include <greatest.h>

TEST parse_lower_transport(void) {

#define CHECK(expected, input)                                                 \
    ASSERT_EQ(                                                                 \
        expected, smolrtsp_parse_lower_transport(CharSlice99_from_str(input)))

    CHECK(SmolRTSP_LowerTransport_UDP, "RTP/AVP/UDP");
    CHECK(SmolRTSP_LowerTransport_TCP, "RTP/AVP/TCP");
    CHECK(SmolRTSP_LowerTransport_UDP, "RTP/AVP");

    CHECK(
        SmolRTSP_LowerTransport_UDP,
        "RTP/AVP/UDP;unicast;client_port=3056-3057");
    CHECK(
        SmolRTSP_LowerTransport_TCP,
        "RTP/AVP/TCP;unicast;client_port=3056-3057");
    CHECK(SmolRTSP_LowerTransport_UDP, "RTP/AVP;unicast;client_port=3056-3057");

    CHECK(-1, "RTP/blah;unicast;client_port=3056-3057");

#undef CHECK

    PASS();
}

TEST parse_header_param(void) {
    CharSlice99 ret;

#define CHECK(param_name, value, expected)                                     \
    do {                                                                       \
        ASSERT_EQ(                                                             \
            0, smolrtsp_parse_header_param(                                    \
                   &ret, param_name, CharSlice99_from_str(value)));            \
        ASSERT(CharSlice99_primitive_eq(ret, CharSlice99_from_str(expected))); \
    } while (0)

    CHECK(
        "client_port=", "RTP/AVP/UDP;unicast;client_port=3056-3057",
        "3056-3057");
    CHECK(
        "client_port=",
        "RTP/AVP/UDP;unicast;client_port=3056-3057;server_port=5002-5003",
        "3056-3057");
    CHECK(
        "server_port=",
        "RTP/AVP/UDP;unicast;client_port=3056-3057;server_port=5002-5003",
        "5002-5003");

#undef CHECK

    ASSERT_EQ(
        -1,
        smolrtsp_parse_header_param(
            &ret, "abracadabra",
            CharSlice99_from_str("RTP/AVP/UDP;unicast;client_port=3056-3057")));

    PASS();
}

TEST parse_range(void) {
    uint8_t interleaved_rtp_port = 0, interleaved_rtcp_port = 0;

    ASSERT_EQ(
        0,
        smolrtsp_parse_range(
            &interleaved_rtp_port, &interleaved_rtcp_port, SCNu8,
            "interleaved=",
            CharSlice99_from_str(
                "RTP/AVP/UDP;unicast;client_port=3056-3057;interleaved=4-5")));
    ASSERT_EQ(4, interleaved_rtp_port);
    ASSERT_EQ(5, interleaved_rtcp_port);

    PASS();
}

TEST parse_port_pair(void) {
    SmolRTSP_PortPair client_port = {0};

    ASSERT_EQ(
        0,
        smolrtsp_parse_port_pair(
            &client_port, "client_port=",
            CharSlice99_from_str(
                "RTP/AVP/UDP;unicast;interleaved=4-5;client_port=3056-3057")));
    ASSERT_EQ(3056, client_port.rtp_port);
    ASSERT_EQ(3057, client_port.rtcp_port);

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
    RUN_TEST(parse_header_param);
    RUN_TEST(parse_range);
    RUN_TEST(parse_port_pair);
    RUN_TEST(interleaved_header);
    RUN_TEST(parse_interleaved_header);
}
