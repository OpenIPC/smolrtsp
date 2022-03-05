#include <smolrtsp/types/response.h>

#define TEST_PARSE_INIT_TYPE(result) result = SmolRTSP_Response_uninit()

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_Response)

TEST parse_response(void) {
    const SmolRTSP_Response expected = {
        .start_line =
            {
                .version = {.major = 1, .minor = 1},
                .code = SMOLRTSP_STATUS_OK,
                .reason = CharSlice99_from_str("OK"),
            },
        .header_map = SmolRTSP_HeaderMap_from_array({
            {SMOLRTSP_HEADER_C_SEQ, CharSlice99_from_str("123")},
            {SMOLRTSP_HEADER_CONTENT_LENGTH, CharSlice99_from_str("10")},
            {SMOLRTSP_HEADER_ACCEPT_LANGUAGE, CharSlice99_from_str("English")},
            {SMOLRTSP_HEADER_CONTENT_TYPE,
             CharSlice99_from_str("application/octet-stream")},
        }),
        .body = CharSlice99_from_str("0123456789"),
        .cseq = 123,
    };

    TEST_PARSE(
        "RTSP/1.1 200 OK\r\n"
        "CSeq: 123\r\n"
        "Content-Length: 10\r\n"
        "Accept-Language: English\r\n"
        "Content-Type: application/octet-stream\r\n"
        "\r\n0123456789",
        expected);

    PASS();
}

TEST serialize_response(void) {
    char buffer[500] = {0};

    const SmolRTSP_Response response = {
        .start_line =
            {
                .version = {1, 0},
                .code = SMOLRTSP_STATUS_OK,
                .reason = CharSlice99_from_str("OK"),
            },
        .header_map = SmolRTSP_HeaderMap_from_array({
            {SMOLRTSP_HEADER_DATE,
             CharSlice99_from_str("Thu, 05 Jun 1997 18:57:19 GMT")},
            {SMOLRTSP_HEADER_CONTENT_TYPE,
             CharSlice99_from_str("application/octet-stream")},
        }),
        .body = CharSlice99_from_str("1234567890"),
        .cseq = 456,
    };

    const ssize_t ret =
        SmolRTSP_Response_serialize(&response, smolrtsp_string_writer(buffer));

    const char *expected =
        "RTSP/1.0 200 OK\r\n"
        "CSeq: 456\r\n"
        "Content-Length: 10\r\n"
        "Date: Thu, 05 Jun 1997 18:57:19 GMT\r\n"
        "Content-Type: application/octet-stream\r\n"
        "\r\n1234567890";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(types_response) {
    RUN_TEST(parse_response);
    RUN_TEST(serialize_response);
}
