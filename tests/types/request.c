#include <smolrtsp/types/request.h>

#include <greatest.h>

TEST parse_request(void) {
    const SmolRTSP_Request expected = {
        .start_line =
            {
                .method = SMOLRTSP_METHOD_DESCRIBE,
                .uri = CharSlice99_from_str("http://example.com"),
                .version = {.major = 1, .minor = 1},
            },
        .header_map = SmolRTSP_HeaderMap_from_array((SmolRTSP_Header[]){
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

    SmolRTSP_RequestParseState state = 0;

    const size_t headers_len = 3;
    SmolRTSP_Header *headers = malloc(sizeof headers[0] * headers_len);
    assert(headers);
    SmolRTSP_HeaderMap header_map = {.headers = headers, .len = 0, .capacity = headers_len};

    SmolRTSP_Request result = {.header_map = header_map};
    SmolRTSP_ParseResult res;

#define CHECK(data, expected_state)                                                                \
    do {                                                                                           \
        res = SmolRTSP_Request_parse(&result, CharSlice99_from_str(data), &state);                 \
        ASSERT_EQ(SmolRTSP_RequestParseState_##expected_state, state);                             \
    } while (0)

    CHECK("DESCRIBE http://example.com RTSP/1.1\r\n", HeaderMap);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    ASSERT(SmolRTSP_RequestLine_eq(result.start_line, expected.start_line));

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
    ASSERT(SmolRTSP_Request_eq(result, expected));

#undef CHECK

    // Test whole request.
    {
        SmolRTSP_RequestParseState state = 0;

        const size_t headers_len = 3;
        SmolRTSP_Header *headers = malloc(sizeof headers[0] * headers_len);
        assert(headers);
        SmolRTSP_HeaderMap header_map = {.headers = headers, .len = 0, .capacity = headers_len};

        SmolRTSP_Request result = {.header_map = header_map};
        const CharSlice99 req = CharSlice99_from_str(
            "DESCRIBE http://example.com RTSP/1.1\r\nAccept-Language: English\r\n\r\n");

        res = SmolRTSP_Request_parse(&result, req, &state);

        match(res) {
            of(SmolRTSP_ParseResult_Success, status) {
                ASSERT(status->is_complete);
                ASSERT(req.len == status->offset);
            }
            of(SmolRTSP_ParseResult_Failure, e) {
                SmolRTSP_ParseError_print(*e, smolrtsp_file_writer(stderr));
                ASSERT(false);
            }
        }

        free(headers);
    }

    free(headers);
    PASS();
}

TEST serialize_request(void) {
    char buffer[500] = {0};

    const SmolRTSP_Request request = {
        .start_line =
            {
                .method = SMOLRTSP_METHOD_DESCRIBE,
                .uri = CharSlice99_from_str("http://example.com"),
                .version = (SmolRTSP_RtspVersion){1, 0},
            },
        .header_map = SmolRTSP_HeaderMap_from_array((SmolRTSP_Header[]){
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

    const ssize_t ret = SmolRTSP_Request_serialize(request, smolrtsp_string_writer(buffer));

    const char *expected =
        "DESCRIBE http://example.com RTSP/1.0\r\nContent-Length: 123\r\nContent-Type: "
        "application/octet-stream\r\n\r\n1234567890";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(request) {
    RUN_TEST(parse_request);
    RUN_TEST(serialize_request);
}
