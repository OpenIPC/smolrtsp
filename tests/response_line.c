#include <smolrtsp/response_line.h>

#include "nala/nala.h"

TEST(parse_response_line) {
    const SmolRTSP_ResponseLine expected = {
        .version = {.major = 1, .minor = 1},
        .code = SMOLRTSP_STATUS_CODE_OK,
        .reason = CharSlice99_from_str("OK"),
    };

    SmolRTSP_ResponseLineParseState state = SMOLRTSP_RESPONSE_LINE_PARSE_STATE_INIT;
    SmolRTSP_ResponseLine result;
    SmolRTSP_ParseResult res;

#define CHECK(data, expected_state)                                                                \
    do {                                                                                           \
        res = SmolRTSP_ResponseLine_parse(&result, CharSlice99_from_str(data), &state);            \
        ASSERT_EQ(state.tag, SmolRTSP_ResponseLineParseState_##expected_state);                    \
    } while (0)

    CHECK("RTSP/1.1 ", StatusCode);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    assert(SmolRTSP_RtspVersion_eq(result.version, expected.version));

    CHECK("200 ", ReasonPhrase);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    assert(result.code == expected.code);

    CHECK("OK\r\n", Done);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    assert(SmolRTSP_ResponseLine_eq(result, expected));

#undef CHECK
}

TEST(serialize_response_line) {
    char buffer[100] = {0};

    const SmolRTSP_ResponseLine line = {
        .version = (SmolRTSP_RtspVersion){1, 0},
        .code = SMOLRTSP_STATUS_CODE_OK,
        .reason = CharSlice99_from_str("OK"),
    };

    SmolRTSP_ResponseLine_serialize(line, smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(strcmp(buffer, "RTSP/1.0 200 OK\r\n"), 0);
}
