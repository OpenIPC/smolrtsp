#include <smolrtsp/types/sdp.h>

#include <greatest.h>

TEST serialize_sdp_line(void) {
    char buffer[20] = {0};

    SmolRTSP_SdpLine_serialize(
        (SmolRTSP_SdpLine){.ty = SMOLRTSP_SDP_ATTR, .value = CharSlice99_from_str("abc")},
        smolrtsp_string_writer(buffer));
    ASSERT_STR_EQ("a=abc\r\n", buffer);

    PASS();
}

TEST sdp_printf(void) {
    char buffer[20] = {0};

    smolrtsp_sdp_printf(
        SMOLRTSP_SDP_ATTR, smolrtsp_string_writer(buffer), "abc %d @ %s", 123, "def");
    ASSERT_STR_EQ("a=abc 123 @ def\r\n", buffer);

    PASS();
}

SUITE(sdp) {
    RUN_TEST(serialize_sdp_line);
    RUN_TEST(sdp_printf);
}
