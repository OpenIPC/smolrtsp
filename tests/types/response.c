#include <smolrtsp/types/response.h>

#include <greatest.h>

TEST parse_response(void) {
    const SmolRTSP_Response expected = {
        .start_line =
            {
                .version = {.major = 1, .minor = 1},
                .code = SMOLRTSP_STATUS_CODE_OK,
                .reason = CharSlice99_from_str("OK"),
            },
        .header_map = SmolRTSP_HeaderMap_from_array({
            {
                SMOLRTSP_HEADER_CONTENT_LENGTH,
                CharSlice99_from_str("10"),
            },
            {
                SMOLRTSP_HEADER_ACCEPT_LANGUAGE,
                CharSlice99_from_str("English"),
            },
            {
                SMOLRTSP_HEADER_CONTENT_TYPE,
                CharSlice99_from_str("application/octet-stream"),
            },
        }),
        .body = CharSlice99_from_str("0123456789"),
    };

    SmolRTSP_ResponseParseState state = 0;

    SmolRTSP_Response result = {.header_map = SmolRTSP_HeaderMap_empty()};
    SmolRTSP_ParseResult ret;

    ret = SmolRTSP_Response_parse(
        &result,
        CharSlice99_from_str("RTSP/1.1 200 OK\r\nContent-Length: 10\r\nAccept-Language: "
                             "English\r\nContent-Type: application/octet-stream\r\n\r\n0123456789"),
        &state);
    ASSERT(SmolRTSP_ParseResult_is_complete(ret));
    ASSERT_EQ(SmolRTSP_ResponseParseState_Done, state);
    ASSERT(SmolRTSP_Response_eq(result, expected));

    PASS();
}

TEST serialize_response(void) {
    char buffer[500] = {0};

    const SmolRTSP_Response response = {
        .start_line =
            {
                .version = (SmolRTSP_RtspVersion){1, 0},
                .code = SMOLRTSP_STATUS_CODE_OK,
                .reason = CharSlice99_from_str("OK"),
            },
        .header_map = SmolRTSP_HeaderMap_from_array({
            {
                SMOLRTSP_HEADER_CONTENT_LENGTH,
                CharSlice99_from_str("123"),
            },
            {
                SMOLRTSP_HEADER_CONTENT_TYPE,
                CharSlice99_from_str("application/octet-stream"),
            },
        }),
        .body = CharSlice99_from_str("1234567890"),
    };

    const ssize_t ret = SmolRTSP_Response_serialize(response, smolrtsp_string_writer(buffer));

    const char *expected =
        "RTSP/1.0 200 OK\r\nContent-Length: 123\r\nContent-Type: "
        "application/octet-stream\r\n\r\n1234567890";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(response) {
    RUN_TEST(parse_response);
    RUN_TEST(serialize_response);
}
