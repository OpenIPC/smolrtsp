#include <smolrtsp/types/sdp.h>

#include <greatest.h>

TEST serialize_sdp_line(void) {
    char buffer[20] = {0};

    SmolRTSP_SdpLine_serialize(
        (SmolRTSP_SdpLine){.ty = 'a', .value = CharSlice99_from_str("abc")},
        smolrtsp_char_buffer_writer, buffer);
    ASSERT_EQ(strcmp(buffer, "a=abc\r\n"), 0);

    PASS();
}

SUITE(sdp) {
    RUN_TEST(serialize_sdp_line);
}
