#include <smolrtsp/response.h>

#include "nala.h"

TEST(deserialize_response) {
    const SmolRTSP_Response expected = {
        .start_line =
            {
                .version = {.major = 1, .minor = 1},
                .code = SMOLRTSP_STATUS_CODE_OK,
                .reason = Slice99_from_str("OK"),
            },
        .header_map = SMOLRTSP_HEADER_MAP_FROM_ARRAY((SmolRTSP_Header[]){
            {
                SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
                Slice99_from_str("10"),
            },
            {
                SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
                Slice99_from_str("English"),
            },
            {
                SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
                Slice99_from_str("application/octet-stream"),
            },
        }),
        .body = Slice99_from_str("0123456789"),
    };

    SmolRTSP_ResponseDeserializerState state = SMOLRTSP_RESPONSE_DESERIALIZER_START_STATE;
    SmolRTSP_Response result = {
        .header_map =
            {
                .headers = (SmolRTSP_Header[3]){0},
                .len = 0,
                .size = 3,
            },
    };
    SmolRTSP_DeserializeResult res;

#define CHECK(data, expected_res, expected_state)                                                  \
    res = SmolRTSP_Response_deserialize(&result, (Slice99[]){Slice99_from_str(data)}, &state);     \
    ASSERT_EQ(res, SmolRTSP_DeserializeResult##expected_res);                                      \
    ASSERT_EQ(state.tag, SmolRTSP_ResponseDeserializerState##expected_state)

    CHECK("RTSP/1.1 200 OK\r\n", Pending, HeaderMap);
    assert(SmolRTSP_ResponseLine_eq(result.start_line, expected.start_line));

    CHECK("Content-Length: 10\r\n", Pending, HeaderMap);
    assert(SmolRTSP_Header_eq(result.header_map.headers[0], expected.header_map.headers[0]));

    CHECK("Accept-Language: English\r\n", Pending, HeaderMap);
    assert(SmolRTSP_Header_eq(result.header_map.headers[1], expected.header_map.headers[1]));

    CHECK("Content-Type: application/octet-stream\r\n", Pending, HeaderMap);
    assert(SmolRTSP_Header_eq(result.header_map.headers[2], expected.header_map.headers[2]));

    CHECK("\r\n0123456789", Ok, Done);
    assert(SmolRTSP_Response_eq(result, expected));

#undef CHECK
}

TEST(serialize_response) {
    char buffer[500] = {0};

    const SmolRTSP_Response response = {
        .start_line =
            {
                .version = SmolRTSP_RTSPVersion_new(1, 0),
                .code = SMOLRTSP_STATUS_CODE_OK,
                .reason = Slice99_from_str("OK"),
            },
        .header_map = SMOLRTSP_HEADER_MAP_FROM_ARRAY((SmolRTSP_Header[]){
            {
                SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
                Slice99_from_str("123"),
            },
            {
                SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
                Slice99_from_str("application/octet-stream"),
            },
        }),
        .body = Slice99_from_str("1234567890"),
    };

    SmolRTSP_Response_serialize(response, smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(
        strcmp(
            buffer,
            "RTSP/1.0 200 OK\r\nContent-Length: 123\r\nContent-Type: "
            "application/octet-stream\r\n\r\n1234567890"),
        0);
}
