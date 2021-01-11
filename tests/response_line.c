#include <smolrtsp/response_line.h>

#include "nala.h"

static void check(Slice99 response_line, SmolRTSP_ResponseLine expected) {
    SmolRTSP_ResponseLine result;
    SmolRTSP_ResponseLineDeserializerState state = SMOLRTSP_RESPONSE_LINE_DESERIALIZER_START_STATE;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_ResponseLine_deserialize(&result, &response_line, &state);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_ResponseLine_eq(result, expected));
}

TEST(deserialize_response_line) {
    check(
        Slice99_from_str("RTSP/1.1 200 OK\r\n"), (SmolRTSP_ResponseLine){
                                                     .version = {.major = 1, .minor = 1},
                                                     .code = SMOLRTSP_STATUS_CODE_OK,
                                                     .reason = Slice99_from_str("OK"),
                                                 });
}
