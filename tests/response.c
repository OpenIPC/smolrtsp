#include <smolrtsp/response.h>

#include "nala.h"

static void check(Slice99 response, SmolRTSP_Response expected) {
    SmolRTSP_Response result = {
        .header_map =
            {
                .headers = (SmolRTSP_Header[3]){0},
                .len = 0,
                .size = 3,
            },
    };
    SmolRTSP_ResponseDeserializerState state = SmolRTSP_ResponseDeserializerStateResponseLine;
    SmolRTSP_ResponseLineDeserializerState start_line_state =
        SmolRTSP_ResponseLineDeserializerStateRTSPVersion;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_Response_deserialize(&result, &response, &state, &start_line_state);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_Response_eq(result, expected));
}

TEST(deserialize_response) {
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

    const SmolRTSP_Response expected = {
        .start_line =
            {
                .version = {.major = 1, .minor = 1},
                .code = SMOLRTSP_STATUS_CODE_OK,
                .reason = Slice99_from_str("OK"),
            },
        .header_map =
            {
                .len = 3,
                .size = 3,
                .headers = headers,
            },
        .body = Slice99_from_str("0123456789"),
    };

    check(
        Slice99_from_str("RTSP/1.1 200 OK\r\n"
                         "Content-Length: 10\r\nAccept-Language: English\r\nContent-Type: "
                         "application/octet-stream\r\n\r\n"
                         "0123456789"),
        expected);
}
