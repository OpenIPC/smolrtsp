#include <smolrtsp/request.h>

#include "nala/nala.h"

TEST(parse_request) {
    const SmolRTSP_Request expected = {
        .start_line =
            {
                .method = SMOLRTSP_METHOD_DESCRIBE,
                .uri = CharSlice99_from_str("http://example.com"),
                .version = {.major = 1, .minor = 1},
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

    SmolRTSP_RequestParseState state = SMOLRTSP_REQUEST_PARSE_STATE_INIT;
    SmolRTSP_Request result = {.header_map = SmolRTSP_HeaderMap_with_capacity(3)};
    SmolRTSP_ParseResult res;

#define CHECK(data, expected_res, expected_state)                                                  \
    res = SmolRTSP_Request_parse(&result, (CharSlice99[]){CharSlice99_from_str(data)}, &state);    \
    ASSERT_EQ(res, SmolRTSP_ParseResult_##expected_res);                                           \
    ASSERT_EQ(state.tag, SmolRTSP_RequestParseState_##expected_state)

    CHECK("DESCRIBE http://example.com RTSP/1.1\r\n", Pending, HeaderMap);
    assert(SmolRTSP_RequestLine_eq(result.start_line, expected.start_line));

    CHECK("Content-Length: 10\r\n", Pending, HeaderMap);
    assert(SmolRTSP_Header_eq(result.header_map.headers[0], expected.header_map.headers[0]));

    CHECK("Accept-Language: English\r\n", Pending, HeaderMap);
    assert(SmolRTSP_Header_eq(result.header_map.headers[1], expected.header_map.headers[1]));

    CHECK("Content-Type: application/octet-stream\r\n", Pending, HeaderMap);
    assert(SmolRTSP_Header_eq(result.header_map.headers[2], expected.header_map.headers[2]));

    CHECK("\r\n0123456789", Ok, Done);
    assert(SmolRTSP_Request_eq(result, expected));

#undef CHECK
}

TEST(serialize_request) {
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

    SmolRTSP_Request_serialize(request, smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(
        strcmp(
            buffer,
            "DESCRIBE http://example.com RTSP/1.0\r\nContent-Length: 123\r\nContent-Type: "
            "application/octet-stream\r\n\r\n1234567890"),
        0);
}
