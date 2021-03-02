#include <smolrtsp/util.h>

#include "nala.h"

TEST(parse_lower_transport) {
    SmolRTSP_LowerTransport lower_transport;

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport, Slice99_from_str("RTP/AVP/UDP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_UDP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport, Slice99_from_str("RTP/AVP/TCP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_TCP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport, Slice99_from_str("RTP/AVP;unicast;client_port=3056-3057")),
        0);
    ASSERT_EQ(lower_transport, SmolRTSP_LowerTransport_UDP);

    ASSERT_EQ(
        SmolRTSP_parse_lower_transport(
            &lower_transport, Slice99_from_str("RTP/blah;unicast;client_port=3056-3057")),
        -1);
}
