#include <smolrtsp/types/request_line.h>

#include <greatest.h>

TEST parse_request_line(void) {
    const SmolRTSP_RequestLine expected = {
        .method = SMOLRTSP_METHOD_DESCRIBE,
        .uri = CharSlice99_from_str("http://example.com"),
        .version = {.major = 1, .minor = 1},
    };

    SmolRTSP_RequestLine result;
    SmolRTSP_ParseResult res;

    res = SmolRTSP_RequestLine_parse(&result, CharSlice99_from_str("DESCRIBE "));
    ASSERT(SmolRTSP_ParseResult_is_partial(res));

    res = SmolRTSP_RequestLine_parse(&result, CharSlice99_from_str("DESCRIBE http://example.com "));
    ASSERT(SmolRTSP_ParseResult_is_partial(res));

    res = SmolRTSP_RequestLine_parse(
        &result, CharSlice99_from_str("DESCRIBE http://example.com RTSP/1.1\r"));
    ASSERT(SmolRTSP_ParseResult_is_partial(res));

    res = SmolRTSP_RequestLine_parse(
        &result, CharSlice99_from_str("DESCRIBE http://example.com RTSP/1.1\r\n"));
    ASSERT(SmolRTSP_ParseResult_is_complete(res));

    ASSERT(SmolRTSP_RequestLine_eq(expected, result));

    PASS();
}

TEST serialize_request_line(void) {
    char buffer[100] = {0};

    const SmolRTSP_RequestLine line = {
        .method = SMOLRTSP_METHOD_DESCRIBE,
        .uri = CharSlice99_from_str("http://example.com"),
        .version = (SmolRTSP_RtspVersion){1, 0},
    };

    const ssize_t ret = SmolRTSP_RequestLine_serialize(line, smolrtsp_string_writer(buffer));

    const char *expected = "DESCRIBE http://example.com RTSP/1.0\r\n";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(request_line) {
    RUN_TEST(parse_request_line);
    RUN_TEST(serialize_request_line);
}
