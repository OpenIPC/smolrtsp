#include <smolrtsp/deserializers/request_line.h>

#include <string.h>

#include "../nala.h"

static void check(const char *line, SmolRTSP_RequestLine expected) {
    SmolRTSP_RequestLineDeserializer *deser = SmolRTSP_RequestLineDeserializer_new();
    ASSERT_NE(deser, NULL);

    Slice99 data = Slice99_from_str((char *)line);
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_RequestLineDeserializer_deserialize(deser, &data);
    const SmolRTSP_RequestLine inner = SmolRTSP_RequestLineDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_RequestLineDeserializer_bytes_read(deser);
    const SmolRTSP_RequestLineDeserializerState state =
        SmolRTSP_RequestLineDeserializer_state(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(state.is_ok);
    ASSERT_EQ(state.in_progress, SmolRTSP_RequestLineDeserializerStateInProgressDone);
    ASSERT_EQ(bytes_read, strlen(line));
    ASSERT(SmolRTSP_RequestLine_eq(&inner, &expected));

    SmolRTSP_RequestLineDeserializer_free(deser);
}

TEST(test_deserializers_request_line) {
    check(
        "DESCRIBE http://example.com RTSP/1.1" SMOLRTSP_CRLF,
        (SmolRTSP_RequestLine){
            .method = SMOLRTSP_METHOD_DESCRIBE,
            .uri = Slice99_from_str("http://example.com"),
            .version = {.major = 1, .minor = 1},
        });
}
