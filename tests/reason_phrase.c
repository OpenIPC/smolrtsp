#include <smolrtsp/reason_phrase.h>

#include "nala/nala.h"

static void assert_pending(CharSlice99 input) {
    SmolRTSP_ReasonPhrase result;
    SmolRTSP_ParseResult res = SmolRTSP_ReasonPhrase_parse(&result, &input);
    ASSERT_EQ(res, SmolRTSP_ParseResult_Pending);
}

static void assert_ok(CharSlice99 input, SmolRTSP_ReasonPhrase expected) {
    SmolRTSP_ReasonPhrase result;
    SmolRTSP_ParseResult res = SmolRTSP_ReasonPhrase_parse(&result, &input);
    ASSERT_EQ(res, SmolRTSP_ParseResult_Ok);
    ASSERT(CharSlice99_primitive_eq(result, expected));
}

TEST(parse_reason_phrase) {
    const CharSlice99 input = CharSlice99_from_str("Moved Temporarily\r\n");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(CharSlice99_update_len(input, i));
    }

    assert_ok(input, CharSlice99_from_str("Moved Temporarily"));
}
