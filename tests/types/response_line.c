#include <smolrtsp/types/response_line.h>

#include <greatest.h>

TEST parse_response_line(void) {
    const SmolRTSP_ResponseLine expected = {
        .version = {.major = 1, .minor = 1},
        .code = SMOLRTSP_STATUS_CODE_OK,
        .reason = CharSlice99_from_str("OK"),
    };

    SmolRTSP_ResponseLine result;
    SmolRTSP_ParseResult res;

    res = SmolRTSP_ResponseLine_parse(&result, CharSlice99_from_str("RTSP/1.1 "));
    ASSERT(SmolRTSP_ParseResult_is_partial(res));

    res = SmolRTSP_ResponseLine_parse(&result, CharSlice99_from_str("RTSP/1.1 200 "));
    ASSERT(SmolRTSP_ParseResult_is_partial(res));

    res = SmolRTSP_ResponseLine_parse(&result, CharSlice99_from_str("RTSP/1.1 200 OK\r\n"));
    ASSERT(SmolRTSP_ParseResult_is_complete(res));

    ASSERT(SmolRTSP_ResponseLine_eq(expected, result));

    PASS();
}

TEST serialize_response_line(void) {
    char buffer[100] = {0};

    const SmolRTSP_ResponseLine line = {
        .version = (SmolRTSP_RtspVersion){1, 0},
        .code = SMOLRTSP_STATUS_CODE_OK,
        .reason = CharSlice99_from_str("OK"),
    };

    const ssize_t ret = SmolRTSP_ResponseLine_serialize(line, smolrtsp_string_writer(buffer));

    const char *expected = "RTSP/1.0 200 OK\r\n";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(response_line) {
    RUN_TEST(parse_response_line);
    RUN_TEST(serialize_response_line);
}
