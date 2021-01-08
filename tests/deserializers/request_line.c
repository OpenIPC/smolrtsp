#include <smolrtsp/request_line.h>

#include <string.h>

#include "../nala.h"

static void check(const char *line, SmolRTSP_RequestLine expected) {
    Slice99 data = Slice99_from_str((char *)line);
    SmolRTSP_RequestLine result;
    SmolRTSP_RequestLineDeserializerState state = SmolRTSP_RequestLineDeserializerStateMethod;
    const SmolRTSP_DeserializeResult res = SmolRTSP_RequestLine_deserialize(&result, &data, &state);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(state, SmolRTSP_RequestLineDeserializerStateDone);
    ASSERT(SmolRTSP_RequestLine_eq(result, expected));
}

TEST(test_deserializers_request_line) {
    check(
        "DESCRIBE http://example.com RTSP/1.1"
        "\r\n",
        (SmolRTSP_RequestLine){
            .method = SMOLRTSP_METHOD_DESCRIBE,
            .uri = Slice99_from_str("http://example.com"),
            .version = {.major = 1, .minor = 1},
        });
}
