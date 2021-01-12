#include <smolrtsp/request_line.h>

#include "nala.h"

TEST(deserialize_request_line) {
    const SmolRTSP_RequestLine expected = {
        .method = SMOLRTSP_METHOD_DESCRIBE,
        .uri = Slice99_from_str("http://example.com"),
        .version = {.major = 1, .minor = 1},
    };

    SmolRTSP_RequestLineDeserializerState state = SMOLRTSP_REQUEST_LINE_DESERIALIZER_START_STATE;
    SmolRTSP_RequestLine result;
    SmolRTSP_DeserializeResult res;

    res = SmolRTSP_RequestLine_deserialize(
        &result, (Slice99[]){Slice99_from_str("DESCRIBE ")}, &state);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
    ASSERT_EQ(state.tag, SmolRTSP_RequestLineDeserializerStateRequestURI);
    assert(Slice99_primitive_eq(result.method, expected.method));

    res = SmolRTSP_RequestLine_deserialize(
        &result, (Slice99[]){Slice99_from_str("http://example.com ")}, &state);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
    ASSERT_EQ(state.tag, SmolRTSP_RequestLineDeserializerStateRTSPVersion);
    assert(Slice99_primitive_eq(result.uri, expected.uri));

    res = SmolRTSP_RequestLine_deserialize(
        &result, (Slice99[]){Slice99_from_str("RTSP/1.1\r")}, &state);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
    ASSERT_EQ(state.tag, SmolRTSP_RequestLineDeserializerStateCRLF);
    assert(SmolRTSP_RequestLine_eq(result, expected));

    res = SmolRTSP_RequestLine_deserialize(&result, (Slice99[]){Slice99_from_str("\r\n")}, &state);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(state.tag, SmolRTSP_RequestLineDeserializerStateDone);
    assert(SmolRTSP_RequestLine_eq(result, expected));
}
