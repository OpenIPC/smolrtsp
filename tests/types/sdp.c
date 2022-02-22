#include <smolrtsp/types/sdp.h>

#include <greatest.h>

TEST serialize_sdp_line(void) {
    char buffer[20] = {0};

    const ssize_t ret = SmolRTSP_SdpLine_serialize(
        (SmolRTSP_SdpLine){.ty = SMOLRTSP_SDP_ATTR, .value = CharSlice99_from_str("abc")},
        smolrtsp_string_writer(buffer));

    const char *expected = "a=abc\r\n";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

TEST sdp_printf(void) {
    char buffer[20] = {0};

    const char *expected = "a=abc 123 @ def\r\n";

    const ssize_t ret = smolrtsp_sdp_printf(
        SMOLRTSP_SDP_ATTR, smolrtsp_string_writer(buffer), "abc %d @ %s", 123, "def");
    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(sdp) {
    RUN_TEST(serialize_sdp_line);
    RUN_TEST(sdp_printf);
}
