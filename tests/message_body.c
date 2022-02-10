#include <smolrtsp/message_body.h>

#include <greatest.h>

static enum greatest_test_res assert_pending(CharSlice99 input, size_t content_length) {
    SmolRTSP_MessageBody result;
    SmolRTSP_ParseResult res = SmolRTSP_MessageBody_parse(&result, input, content_length);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    PASS();
}

static enum greatest_test_res
assert_ok(CharSlice99 input, SmolRTSP_MessageBody expected, size_t content_length) {
    SmolRTSP_MessageBody result;
    SmolRTSP_ParseResult res = SmolRTSP_MessageBody_parse(&result, input, content_length);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(CharSlice99_primitive_eq(result, expected));
    PASS();
}

TEST parse_message_body(void) {
    const CharSlice99 input = CharSlice99_from_str(" 012345 ~ abc(^*%  D#NIN#3   ");
    const size_t content_length = input.len;

    for (size_t i = 0; i < input.len - 1; i++) {
        CHECK_CALL(assert_pending(CharSlice99_update_len(input, i), content_length));
    }

    CHECK_CALL(assert_ok(input, input, content_length));
    CHECK_CALL(assert_ok(CharSlice99_from_str(""), CharSlice99_from_str(""), 0));

    PASS();
}

SUITE(message_body) {
    RUN_TEST(parse_message_body);
}
