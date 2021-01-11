#include <smolrtsp/request_line.h>

#include <string.h>

#include "nala.h"

static void check(Slice99 line, SmolRTSP_RequestLine expected) {
    SmolRTSP_RequestLine result;
    SmolRTSP_RequestLineDeserializerState state = SMOLRTSP_REQUEST_LINE_DESERIALIZER_START_STATE;
    const SmolRTSP_DeserializeResult res = SmolRTSP_RequestLine_deserialize(&result, &line, &state);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(state.tag, SmolRTSP_RequestLineDeserializerStateDone);
    ASSERT(SmolRTSP_RequestLine_eq(result, expected));
}

TEST(deserialize_request_line) {
    check(
        Slice99_from_str("DESCRIBE http://example.com RTSP/1.1\r\n"),
        (SmolRTSP_RequestLine){
            .method = SMOLRTSP_METHOD_DESCRIBE,
            .uri = Slice99_from_str("http://example.com"),
            .version = {.major = 1, .minor = 1},
        });
}
