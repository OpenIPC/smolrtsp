#include <smolrtsp/types/response_line.h>

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_ResponseLine)

TEST parse_response_line(void) {
    const SmolRTSP_ResponseLine expected = {
        .version = {.major = 1, .minor = 1},
        .code = SMOLRTSP_STATUS_OK,
        .reason = CharSlice99_from_str("OK"),
    };

    TEST_PARSE("RTSP/1.1 200 OK\r\n", expected);

    SmolRTSP_ResponseLine result;

    ASSERT(SmolRTSP_ParseResult_is_failure(SmolRTSP_ResponseLine_parse(
        &result, CharSlice99_from_str("ABRACADABRA/1.1 200 OK\r\n"))));
    ASSERT(SmolRTSP_ParseResult_is_failure(SmolRTSP_ResponseLine_parse(
        &result, CharSlice99_from_str("RTSP/42 200 OK\r\n"))));
    ASSERT(SmolRTSP_ParseResult_is_failure(SmolRTSP_ResponseLine_parse(
        &result, CharSlice99_from_str("RTSP/1.1 ~~~ OK\r\n"))));

    PASS();
}

TEST serialize_response_line(void) {
    char buffer[100] = {0};

    const SmolRTSP_ResponseLine line = {
        .version = (SmolRTSP_RtspVersion){1, 0},
        .code = SMOLRTSP_STATUS_OK,
        .reason = CharSlice99_from_str("OK"),
    };

    const ssize_t ret =
        SmolRTSP_ResponseLine_serialize(&line, smolrtsp_string_writer(buffer));

    const char *expected = "RTSP/1.0 200 OK\r\n";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(types_response_line) {
    RUN_TEST(parse_response_line);
    RUN_TEST(serialize_response_line);
}
