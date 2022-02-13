#include <smolrtsp/types/sdp.h>

#include <greatest.h>

TEST serialize_sdp_line(void) {
    char buffer[20] = {0};

    SmolRTSP_SdpLine_serialize(
        (SmolRTSP_SdpLine){.ty = SMOLRTSP_SDP_TYPE_ATTR, .value = CharSlice99_from_str("abc")},
        smolrtsp_strcat_writer(buffer));
    ASSERT_EQ(strcmp(buffer, "a=abc\r\n"), 0);

    PASS();
}

TEST sdp_append(void) {
    char buffer[20] = {0};

    smolrtsp_sdp_append(SMOLRTSP_SDP_TYPE_ATTR, "abc", smolrtsp_strcat_writer(buffer));
    ASSERT_EQ(strcmp(buffer, "a=abc\r\n"), 0);

    PASS();
}

SUITE(sdp) {
    RUN_TEST(serialize_sdp_line);
    RUN_TEST(sdp_append);
}
