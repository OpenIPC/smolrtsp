#include <smolrtsp/request.h>

#include "nala.h"

TEST(deserialize_request) {
    const SmolRTSP_Request expected = {
        .start_line =
            {
                .method = SMOLRTSP_METHOD_DESCRIBE,
                .uri = Slice99_from_str("http://example.com"),
                .version = {.major = 1, .minor = 1},
            },
        .header_map =
            {
                .len = 3,
                .size = 3,
                .headers =
                    (SmolRTSP_Header[]){
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
                    },
            },
        .body = Slice99_from_str("0123456789"),
    };

    SmolRTSP_RequestDeserializerState state = SMOLRTSP_REQUEST_DESERIALIZER_START_STATE;
    SmolRTSP_Request result = {
        .header_map =
            {
                .headers = (SmolRTSP_Header[3]){0},
                .len = 0,
                .size = 3,
            },
    };
    SmolRTSP_DeserializeResult res;

#define CHECK(data, expected_res, expected_state)                                                  \
    res = SmolRTSP_Request_deserialize(&result, (Slice99[]){Slice99_from_str(data)}, &state);      \
    ASSERT_EQ(res, SmolRTSP_DeserializeResult##expected_res);                                      \
    ASSERT_EQ(state.tag, SmolRTSP_RequestDeserializerState##expected_state)

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
