#include "../../src/aux.h"
#include <smolrtsp/deserializers/request.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header, SmolRTSP_Request expected) {
    SmolRTSP_Header headers[3];
    SmolRTSP_RequestDeserializer *deser = SmolRTSP_RequestDeserializer_new(3, headers);
    ASSERT_NE(deser, NULL);

    const SmolRTSP_DeserializeResult res =
        SmolRTSP_RequestDeserializer_deserialize(deser, SmolRTSP_Slice_new(header, strlen(header)));
    const SmolRTSP_Request inner = SmolRTSP_RequestDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_RequestDeserializer_bytes_read(deser);
    const SmolRTSP_RequestDeserializerState state = SmolRTSP_RequestDeserializer_state(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(header));
    ASSERT_EQ(state, SmolRTSP_RequestDeserializerStateMessageBodyParsed);
    ASSERT(SmolRTSP_Request_eq(&inner, &expected));

    SmolRTSP_RequestDeserializer_free(deser);
}

TEST(test_deserializers_request) {
    SmolRTSP_Header headers[] = {
        {
            SmolRTSP_Slice_from_str(SMOLRTSP_HEADER_NAME_CONTENT_LENGTH),
            SmolRTSP_Slice_from_str("10"),
        },
        {
            SmolRTSP_Slice_from_str(SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE),
            SmolRTSP_Slice_from_str("English"),
        },
        {
            SmolRTSP_Slice_from_str(SMOLRTSP_HEADER_NAME_CONTENT_TYPE),
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
        "DESCRIBE http://example.com RTSP/1.1" SMOLRTSP_CRLF SMOLRTSP_HEADER_NAME_CONTENT_LENGTH
        ": 10" SMOLRTSP_CRLF SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE
        ": English" SMOLRTSP_CRLF SMOLRTSP_HEADER_NAME_CONTENT_TYPE
        ": application/octet-stream" SMOLRTSP_CRLF SMOLRTSP_CRLF "0123456789";

    check(request, expected);
}
