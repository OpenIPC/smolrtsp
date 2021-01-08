#include <smolrtsp/request_line.h>

#include <string.h>

#include "../nala.h"

static void check(const char *line, SmolRTSP_RequestLine expected) {
    Slice99 data = Slice99_from_str((char *)line);
    SmolRTSP_RequestLine result;
    SmolRTSP_RequestLineDeserializerState state = {
        SmolRTSP_RequestLineDeserializerStateMethod,
        true,
    };
    size_t bytes_read = 0;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_RequestLine_deserialize(&result, &data, &bytes_read, &state);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(state.is_ok);
    ASSERT_EQ(state.in_progress, SmolRTSP_RequestLineDeserializerStateDone);
    ASSERT_EQ(bytes_read, strlen(line));
    ASSERT(SmolRTSP_RequestLine_eq(&result, &expected));
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
