#include <smolrtsp/status_code.h>

#include "nala/nala.h"

static void assert_pending(CharSlice99 input) {
    SmolRTSP_StatusCode result;
    SmolRTSP_ParseResult res = SmolRTSP_StatusCode_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
}

static void assert_ok(CharSlice99 input, SmolRTSP_StatusCode expected) {
    SmolRTSP_StatusCode result;
    SmolRTSP_ParseResult res = SmolRTSP_StatusCode_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(result == expected);
}

static void assert_err(CharSlice99 input) {
    SmolRTSP_StatusCode result;
    SmolRTSP_ParseResult res = SmolRTSP_StatusCode_parse(&result, input);
    ASSERT(MATCHES(res, SmolRTSP_ParseResult_Failure));
}

TEST(parse_status_code) {
    const CharSlice99 input = CharSlice99_from_str("404 ");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(CharSlice99_update_len(input, i));
    }

    assert_ok(input, SMOLRTSP_STATUS_CODE_NOT_FOUND);

    assert_err(CharSlice99_from_str("blah"));
    assert_err(CharSlice99_from_str("~ 2424 blah"));
}

TEST(serialize_status_code) {
    char buffer[20] = {0};

    SmolRTSP_StatusCode_serialize(
        SMOLRTSP_STATUS_CODE_NOT_FOUND, smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(strcmp(buffer, "404"), 0);
}
