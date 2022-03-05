#include <smolrtsp/types/rtsp_version.h>

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_RtspVersion)

TEST parse_rtsp_version(void) {
    TEST_PARSE("RTSP/1.1 ", ((SmolRTSP_RtspVersion){1, 1}));
    TEST_PARSE("RTSP/0.0 ", ((SmolRTSP_RtspVersion){0, 0}));
    TEST_PARSE("RTSP/123.200 ", ((SmolRTSP_RtspVersion){123, 200}));
    TEST_PARSE("RTSP/0.200 ", ((SmolRTSP_RtspVersion){0, 200}));

    SmolRTSP_RtspVersion result;

    ASSERT(SmolRTSP_ParseResult_is_failure(
        SmolRTSP_RtspVersion_parse(&result, CharSlice99_from_str("192"))));
    ASSERT(SmolRTSP_ParseResult_is_failure(
        SmolRTSP_RtspVersion_parse(&result, CharSlice99_from_str(" ~ RTSP/"))));

    PASS();
}

TEST serialize_rtsp_version(void) {
    char buffer[20] = {0};

    const ssize_t ret = SmolRTSP_RtspVersion_serialize(
        &(SmolRTSP_RtspVersion){1, 0}, smolrtsp_string_writer(buffer));

    const char *expected = "RTSP/1.0";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(types_rtsp_version) {
    RUN_TEST(parse_rtsp_version);
    RUN_TEST(serialize_rtsp_version);
}
