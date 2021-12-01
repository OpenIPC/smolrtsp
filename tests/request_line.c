#include <smolrtsp/request_line.h>

#include "nala/nala.h"

TEST(deserialize_request_line) {
    const SmolRTSP_RequestLine expected = {
        .method = SMOLRTSP_METHOD_DESCRIBE,
        .uri = CharSlice99_from_str("http://example.com"),
        .version = {.major = 1, .minor = 1},
    };

    SmolRTSP_RequestLineDeserializerState state = SMOLRTSP_REQUEST_LINE_DESERIALIZER_STATE_INIT;
    SmolRTSP_RequestLine result;
    SmolRTSP_ParseResult res;

#define CHECK(data, expected_res, expected_state)                                                  \
    res =                                                                                          \
        SmolRTSP_RequestLine_parse(&result, (CharSlice99[]){CharSlice99_from_str(data)}, &state);  \
    ASSERT_EQ(res, SmolRTSP_ParseResult_##expected_res);                                           \
    ASSERT_EQ(state.tag, SmolRTSP_RequestLineDeserializerState_##expected_state)

    CHECK("DESCRIBE ", Pending, RequestURI);
    assert(CharSlice99_primitive_eq(result.method, expected.method));

    CHECK("http://example.com ", Pending, RtspVersion);
    assert(CharSlice99_primitive_eq(result.uri, expected.uri));

    CHECK("RTSP/1.1\r", Pending, Crlf);
    assert(SmolRTSP_RequestLine_eq(result, expected));

    CHECK("\r\n", Ok, Done);
    assert(SmolRTSP_RequestLine_eq(result, expected));

#undef CHECK
}

TEST(serialize_request_line) {
    char buffer[100] = {0};

    const SmolRTSP_RequestLine line = {
        .method = SMOLRTSP_METHOD_DESCRIBE,
        .uri = CharSlice99_from_str("http://example.com"),
        .version = (SmolRTSP_RtspVersion){1, 0},
    };

    SmolRTSP_RequestLine_serialize(line, smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(strcmp(buffer, "DESCRIBE http://example.com RTSP/1.0\r\n"), 0);
}
