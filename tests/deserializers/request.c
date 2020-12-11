#include "../../src/aux.h"
#include <smolrtsp/deserializers/request.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header, SmolRTSP_Request expected) {
    SmolRTSP_RequestDeserializer *deser = SmolRTSP_RequestDeserializer_new();
    ASSERT_NE(deser, NULL);

    SmolRTSP_DeserializeResult res =
        SmolRTSP_RequestDeserializer_deserialize(deser, SmolRTSP_Slice_new(header, strlen(header)));
    SmolRTSP_Request inner = SmolRTSP_RequestDeserializer_inner(deser);
    size_t bytes_read = SmolRTSP_RequestDeserializer_bytes_read(deser);
    SmolRTSP_RequestDeserializerState state = SmolRTSP_RequestDeserializer_state(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(header));
    ASSERT_EQ(state, SmolRTSP_RequestDeserializerStateMessageBodyParsed);
    ASSERT(SmolRTSP_Request_eq(&inner, &expected));

    SmolRTSP_RequestDeserializer_free(deser);
}

TEST(test_deserializers_request) {
    SmolRTSP_Request expected = {
        .start_line =
            {
                .method = SMOLRTSP_METHOD_DESCRIBE,
                .uri = {"http://example.com"},
                .version = {.major = 1, .minor = 1},
            },
        .header_map =
            {
                .count = 3,
                .headers =
                    {
                        {
                            SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
                            strlen(SMOLRTSP_HEADER_NAME_CONTENT_LENGTH),
                            "10",
                            strlen("10"),
                        },
                        {
                            SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
                            strlen(SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE),
                            "English",
                            strlen("English"),
                        },
                        {
                            SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
                            strlen(SMOLRTSP_HEADER_NAME_CONTENT_TYPE),
                            "application/octet-stream",
                            strlen("application/octet-stream"),
                        },
                    },
            },
        .body = SmolRTSP_Slice_from_str("0123456789"),
    };

    const char *request =
        "DESCRIBE http://example.com RTSP/1.1" CRLF SMOLRTSP_HEADER_NAME_CONTENT_LENGTH
        ": 10" CRLF SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE
        ": English" CRLF SMOLRTSP_HEADER_NAME_CONTENT_TYPE ": application/octet-stream" CRLF CRLF
        "0123456789";

    check(request, expected);
}
