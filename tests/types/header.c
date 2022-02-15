#include <smolrtsp/types/header.h>

#include <greatest.h>

static enum greatest_test_res assert_pending(CharSlice99 input) {
    SmolRTSP_Header result;
    SmolRTSP_ParseResult res = SmolRTSP_Header_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    PASS();
}

static enum greatest_test_res assert_ok(CharSlice99 input, SmolRTSP_Header expected) {
    SmolRTSP_Header result;
    SmolRTSP_ParseResult res = SmolRTSP_Header_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(SmolRTSP_Header_eq(result, expected));
    PASS();
}

static enum greatest_test_res assert_err(CharSlice99 input) {
    SmolRTSP_Header result;
    SmolRTSP_ParseResult res = SmolRTSP_Header_parse(&result, input);
    ASSERT(MATCHES(res, SmolRTSP_ParseResult_Failure));
    PASS();
}

TEST parse_header(void) {
    const CharSlice99 input =
        CharSlice99_from_str("User-Agent: LibVLC/3.0.8 (LIVE555 Streaming Media v2018.02.18)\r\n");

    for (size_t i = 0; i < input.len - 1; i++) {
        CHECK_CALL(assert_pending(CharSlice99_update_len(input, i)));
    }

    CHECK_CALL(assert_ok(
        input, (SmolRTSP_Header){
                   SMOLRTSP_HEADER_USER_AGENT,
                   CharSlice99_from_str("LibVLC/3.0.8 (LIVE555 Streaming Media v2018.02.18)"),
               }));

    CHECK_CALL(assert_err(CharSlice99_from_str("~@~")));

    PASS();
}

TEST serialize_header(void) {
    char buffer[200] = {0};

    const SmolRTSP_Header header = {
        SMOLRTSP_HEADER_CONTENT_LENGTH,
        CharSlice99_from_str("123"),
    };

    const ssize_t ret = SmolRTSP_Header_serialize(header, smolrtsp_string_writer(buffer));

    const char *expected = "Content-Length: 123\r\n";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(header) {
    RUN_TEST(parse_header);
    RUN_TEST(serialize_header);
}
