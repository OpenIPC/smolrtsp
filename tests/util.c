#include <smolrtsp/util.h>

#include "nala/nala.h"

TEST(parse_lower_transport) {
    SmolRTSP_LowerTransport lower_transport;

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport, CharSlice99_from_str("RTP/AVP/UDP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_UDP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport, CharSlice99_from_str("RTP/AVP/TCP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_TCP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport, CharSlice99_from_str("RTP/AVP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_UDP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport, CharSlice99_from_str("RTP/blah;unicast;client_port=3056-3057")),
        -1);
}

TEST(parse_client_port) {
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
                "RTP/AVP/UDP;unicast;client_port=3058-3059;server_port=5002-5003")),
        0);
    ASSERT_EQ(rtp_port, 3058);
    ASSERT_EQ(rtcp_port, 3059);
}

TEST(parse_interleaved_chn_id) {
    int rtp_chn_id, rtcp_chn_id;

    ASSERT_EQ(
        SmolRTSP_parse_interleaved_chn_id(
            &rtp_chn_id, &rtcp_chn_id, CharSlice99_from_str("RTP/AVP/UDP;unicast;interleaved=204")),
        0);
    ASSERT_EQ(rtp_chn_id, 204);
    ASSERT_EQ(rtcp_chn_id, -1);

    ASSERT_EQ(
        SmolRTSP_parse_interleaved_chn_id(
            &rtp_chn_id, &rtcp_chn_id,
            CharSlice99_from_str(
                "RTP/AVP/UDP;unicast;interleaved=3058-3059;server_port=5002-5003")),
        0);
    ASSERT_EQ(rtp_chn_id, 3058);
    ASSERT_EQ(rtcp_chn_id, 3059);
}
