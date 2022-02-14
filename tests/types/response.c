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
        .header_map = SmolRTSP_HeaderMap_from_array((SmolRTSP_Header[]){
            {
                SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
                CharSlice99_from_str("10"),
            },
            {
                SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
                CharSlice99_from_str("English"),
            },
            {
                SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
                CharSlice99_from_str("application/octet-stream"),
            },
        }),
        .body = CharSlice99_from_str("0123456789"),
    };

    SmolRTSP_ResponseParseState state = 0;

    const size_t headers_len = 3;
    SmolRTSP_Header *headers = malloc(sizeof headers[0] * headers_len);
    assert(headers);
    SmolRTSP_HeaderMap header_map = {.headers = headers, .len = 0, .capacity = headers_len};

    SmolRTSP_Response result = {.header_map = header_map};
    SmolRTSP_ParseResult res;

#define CHECK(data, expected_state)                                                                \
    do {                                                                                           \
        res = SmolRTSP_Response_parse(&result, CharSlice99_from_str(data), &state);                \
        ASSERT_EQ(SmolRTSP_ResponseParseState_##expected_state, state);                            \
    } while (0)

    CHECK("RTSP/1.1 200 OK\r\n", HeaderMap);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    ASSERT(SmolRTSP_ResponseLine_eq(result.start_line, expected.start_line));

    CHECK("Content-Length: 10\r\n", HeaderMap);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    ASSERT(SmolRTSP_Header_eq(result.header_map.headers[0], expected.header_map.headers[0]));

    CHECK("Accept-Language: English\r\n", HeaderMap);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    ASSERT(SmolRTSP_Header_eq(result.header_map.headers[1], expected.header_map.headers[1]));

    CHECK("Content-Type: application/octet-stream\r\n", HeaderMap);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    ASSERT(SmolRTSP_Header_eq(result.header_map.headers[2], expected.header_map.headers[2]));

    CHECK("\r\n0123456789", Done);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(SmolRTSP_Response_eq(result, expected));

#undef CHECK

    free(headers);
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
        .header_map = SmolRTSP_HeaderMap_from_array((SmolRTSP_Header[]){
            {
                SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
                CharSlice99_from_str("123"),
            },
            {
                SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
                CharSlice99_from_str("application/octet-stream"),
            },
        }),
        .body = CharSlice99_from_str("1234567890"),
    };

    SmolRTSP_Response_serialize(response, smolrtsp_string_writer(buffer));

    ASSERT_STR_EQ(
        "RTSP/1.0 200 OK\r\nContent-Length: 123\r\nContent-Type: "
        "application/octet-stream\r\n\r\n1234567890",
        buffer);

    PASS();
}

SUITE(response) {
    RUN_TEST(parse_response);
    RUN_TEST(serialize_response);
}
