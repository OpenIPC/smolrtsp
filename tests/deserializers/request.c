#include <smolrtsp/request.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header, SmolRTSP_Request expected) {
    Slice99 data = Slice99_from_str((char *)header);
    SmolRTSP_Request result = {
        .header_map =
            {
                .headers = (SmolRTSP_Header[3]){0},
                .len = 0,
                .size = 3,
            },
    };
    SmolRTSP_RequestDeserializerState state = SmolRTSP_RequestDeserializerStateRequestLine;
    SmolRTSP_RequestLineDeserializerState start_line_state =
        SmolRTSP_RequestLineDeserializerStateMethod;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_Request_deserialize(&result, &data, &state, &start_line_state);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(state, SmolRTSP_RequestDeserializerStateDone);
    ASSERT(SmolRTSP_Request_eq(result, expected));
}

TEST(test_deserializers_request) {
    SmolRTSP_Header headers[] = {
        {
            SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
            Slice99_from_str("10"),
        },
        {
            SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
            Slice99_from_str("English"),
        },
        {
            SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
            Slice99_from_str("application/octet-stream"),
        },
    };

    const SmolRTSP_Request expected = {
        .start_line =
            {
                .method = SMOLRTSP_METHOD_DESCRIBE,
                .uri = Slice99_from_str("http://example.com"),
                .version = {.major = 1, .minor = 1},
            },
        .header_map =
            {
                .len = 3,
                .size = 3,
                .headers = headers,
            },
        .body = Slice99_from_str("0123456789"),
    };

    const char *request =
        "DESCRIBE http://example.com RTSP/1.1\r\n"
        "Content-Length: 10\r\nAccept-Language: English\r\nContent-Type: "
        "application/octet-stream\r\n\r\n"
        "0123456789";

    check(request, expected);
}
