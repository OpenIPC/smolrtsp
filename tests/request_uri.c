#include <smolrtsp/request_uri.h>

#include "nala/nala.h"

static void assert_pending(CharSlice99 input) {
    SmolRTSP_RequestURI result;
    SmolRTSP_ParseResult res = SmolRTSP_RequestURI_parse(&result, &input);
    ASSERT_EQ(res, SmolRTSP_ParseResult_Pending);
}

static void assert_ok(CharSlice99 input, SmolRTSP_RequestURI expected) {
    SmolRTSP_RequestURI result;
    SmolRTSP_ParseResult res = SmolRTSP_RequestURI_parse(&result, &input);
    ASSERT_EQ(res, SmolRTSP_ParseResult_Ok);
    ASSERT(CharSlice99_primitive_eq(result, expected));
}

TEST(parse_request_uri) {
    const CharSlice99 input = CharSlice99_from_str("http://example.com ");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(CharSlice99_update_len(input, i));
    }

    assert_ok(input, CharSlice99_from_str("http://example.com"));
}
