#include "../../src/aux.h"
#include <smolrtsp/deserializers/request_line.h>

#include <string.h>

#include "../nala.h"

static void check(const char *line, SmolRTSP_RequestLine expected) {
    SmolRTSP_RequestLineDeserializer *deser = SmolRTSP_RequestLineDeserializer_new();
    ASSERT_NE(deser, NULL);

    SmolRTSP_DeserializeResult res =
        SmolRTSP_RequestLineDeserializer_deserialize(deser, SmolRTSP_Slice_new(line, strlen(line)));
    SmolRTSP_RequestLine inner = SmolRTSP_RequestLineDeserializer_inner(deser);
    size_t bytes_read = SmolRTSP_RequestLineDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(line));
    ASSERT(SmolRTSP_RequestLine_eq(&inner, &expected));

    SmolRTSP_RequestLineDeserializer_free(deser);
}

TEST(test_deserializers_request_line) {
    check(
        "DESCRIBE http://example.com RTSP/1.1" CRLF, (SmolRTSP_RequestLine){
                                                         .method = SMOLRTSP_METHOD_DESCRIBE,
                                                         .uri = {"http://example.com"},
                                                         .version = {.major = 1, .minor = 1},
                                                     });
}
