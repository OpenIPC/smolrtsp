#include <smolrtsp/types/request.h>

#define TEST_PARSE_INIT_TYPE(result) result.header_map = SmolRTSP_HeaderMap_empty()

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_Request)

TEST parse_request(void) {
    const SmolRTSP_Request expected = {
        .start_line =
            {
                .method = SMOLRTSP_METHOD_DESCRIBE,
                .uri = CharSlice99_from_str("http://example.com"),
                .version = {.major = 1, .minor = 1},
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

    TEST_PARSE(
        "DESCRIBE http://example.com RTSP/1.1\r\nContent-Length: 10\r\nAccept-Language: "
        "English\r\nContent-Type: application/octet-stream\r\n\r\n0123456789",
        expected);

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

    const ssize_t ret =
        SmolRTSP_Request_serialize(request, smolrtsp_string_writer(buffer));

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
