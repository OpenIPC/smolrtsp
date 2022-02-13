#include <smolrtsp/types/request_line.h>

#include <greatest.h>

TEST parse_request_line(void) {
    const SmolRTSP_RequestLine expected = {
        .method = SMOLRTSP_METHOD_DESCRIBE,
        .uri = CharSlice99_from_str("http://example.com"),
        .version = {.major = 1, .minor = 1},
    };

    SmolRTSP_RequestLineParseState state = SMOLRTSP_REQUEST_LINE_PARSE_STATE_INIT;
    SmolRTSP_RequestLine result;
    SmolRTSP_ParseResult res;

#define CHECK(data, expected_state)                                                                \
    do {                                                                                           \
        res = SmolRTSP_RequestLine_parse(&result, CharSlice99_from_str(data), &state);             \
        ASSERT_EQ(state.tag, SmolRTSP_RequestLineParseState_##expected_state);                     \
    } while (0)

    CHECK("DESCRIBE ", RequestURI);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    ASSERT(CharSlice99_primitive_eq(result.method, expected.method));

    CHECK("http://example.com ", RtspVersion);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    ASSERT(CharSlice99_primitive_eq(result.uri, expected.uri));

    CHECK("RTSP/1.1\r", Crlf);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    ASSERT(SmolRTSP_RequestLine_eq(result, expected));

    CHECK("\r\n", Done);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(SmolRTSP_RequestLine_eq(result, expected));

#undef CHECK

    PASS();
}

TEST serialize_request_line(void) {
    char buffer[100] = {0};

    const SmolRTSP_RequestLine line = {
        .method = SMOLRTSP_METHOD_DESCRIBE,
        .uri = CharSlice99_from_str("http://example.com"),
        .version = (SmolRTSP_RtspVersion){1, 0},
    };

    SmolRTSP_RequestLine_serialize(line, smolrtsp_strcat_writer(buffer));

    ASSERT_EQ(strcmp(buffer, "DESCRIBE http://example.com RTSP/1.0\r\n"), 0);

    PASS();
}

SUITE(request_line) {
    RUN_TEST(parse_request_line);
    RUN_TEST(serialize_request_line);
}
