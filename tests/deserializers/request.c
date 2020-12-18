#include "../../src/aux.h"
#include <smolrtsp/deserializers/request.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header, SmolRTSP_Request expected) {
    SmolRTSP_Header headers[3];
    SmolRTSP_RequestDeserializer *deser = SmolRTSP_RequestDeserializer_new(3, headers);
    ASSERT_NE(deser, NULL);

    SmolRTSP_Slice data = SmolRTSP_Slice_from_str(header);
    const SmolRTSP_DeserializeResult res = SmolRTSP_RequestDeserializer_deserialize(deser, &data);
    const SmolRTSP_Request inner = SmolRTSP_RequestDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_RequestDeserializer_bytes_read(deser);
    const SmolRTSP_RequestDeserializerState state = SmolRTSP_RequestDeserializer_state(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(header));
    ASSERT(state.is_ok);
    ASSERT_EQ(state.in_progress, SmolRTSP_RequestDeserializerStateInProgressDone);
    ASSERT(SmolRTSP_Request_eq(&inner, &expected));

    SmolRTSP_RequestDeserializer_free(deser);
}

TEST(test_deserializers_request) {
    SmolRTSP_Header headers[] = {
        {
            SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
            SmolRTSP_Slice_from_str("10"),
        },
        {
            SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
            SmolRTSP_Slice_from_str("English"),
        },
        {
            SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
            SmolRTSP_Slice_from_str("application/octet-stream"),
        },
    };

    const SmolRTSP_Request expected = {
        .start_line =
            {
                .method = SMOLRTSP_METHOD_DESCRIBE,
                .uri = SmolRTSP_Slice_from_str("http://example.com"),
                .version = {.major = 1, .minor = 1},
            },
        .header_map =
            {
                .len = 3,
                .size = 3,
                .headers = headers,
            },
        .body = SmolRTSP_Slice_from_str("0123456789"),
    };

    const char *request =
        "DESCRIBE http://example.com RTSP/1.1\r\n"
        "Content-Length: 10\r\nAccept-Language: English\r\nContent-Type: "
        "application/octet-stream\r\n\r\n"
        "0123456789";

    check(request, expected);
}
