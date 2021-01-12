#include <smolrtsp/response_line.h>

#include "nala.h"

TEST(deserialize_response_line) {
    const SmolRTSP_ResponseLine expected = {
        .version = {.major = 1, .minor = 1},
        .code = SMOLRTSP_STATUS_CODE_OK,
        .reason = Slice99_from_str("OK"),
    };

    SmolRTSP_ResponseLineDeserializerState state = SMOLRTSP_RESPONSE_LINE_DESERIALIZER_START_STATE;
    SmolRTSP_ResponseLine result;
    SmolRTSP_DeserializeResult res;

    res = SmolRTSP_ResponseLine_deserialize(
        &result, (Slice99[]){Slice99_from_str("RTSP/1.1 ")}, &state);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
    ASSERT_EQ(state.tag, SmolRTSP_ResponseLineDeserializerStateStatusCode);
    assert(SmolRTSP_RTSPVersion_eq(result.version, expected.version));

    res = SmolRTSP_ResponseLine_deserialize(&result, (Slice99[]){Slice99_from_str("200 ")}, &state);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
    ASSERT_EQ(state.tag, SmolRTSP_ResponseLineDeserializerStateReasonPhrase);
    assert(result.code == expected.code);

    res =
        SmolRTSP_ResponseLine_deserialize(&result, (Slice99[]){Slice99_from_str("OK\r\n")}, &state);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(state.tag, SmolRTSP_ResponseLineDeserializerStateDone);
    assert(SmolRTSP_ResponseLine_eq(result, expected));
}
