#include <smolrtsp/method.h>

#include <greatest.h>

static enum greatest_test_res assert_pending(CharSlice99 input) {
    SmolRTSP_Method result;
    SmolRTSP_ParseResult res = SmolRTSP_Method_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    PASS();
}

static enum greatest_test_res assert_ok(CharSlice99 input, SmolRTSP_Method expected) {
    SmolRTSP_Method result;
    SmolRTSP_ParseResult res = SmolRTSP_Method_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(CharSlice99_primitive_eq(result, expected));
    PASS();
}

static enum greatest_test_res assert_err(CharSlice99 input) {
    SmolRTSP_Method result;
    SmolRTSP_ParseResult res = SmolRTSP_Method_parse(&result, input);
    ASSERT(MATCHES(res, SmolRTSP_ParseResult_Failure));
    PASS();
}

TEST parse_method(void) {
    CHECK_CALL(assert_ok(CharSlice99_from_str("OPTIONS "), SMOLRTSP_METHOD_OPTIONS));
    CHECK_CALL(assert_ok(CharSlice99_from_str("DESCRIBE "), SMOLRTSP_METHOD_DESCRIBE));
    CHECK_CALL(assert_ok(CharSlice99_from_str("ANNOUNCE "), SMOLRTSP_METHOD_ANNOUNCE));
    CHECK_CALL(assert_ok(CharSlice99_from_str("SETUP "), SMOLRTSP_METHOD_SETUP));
    CHECK_CALL(assert_ok(CharSlice99_from_str("PLAY "), SMOLRTSP_METHOD_PLAY));
    CHECK_CALL(assert_ok(CharSlice99_from_str("PAUSE "), SMOLRTSP_METHOD_PAUSE));
    CHECK_CALL(assert_ok(CharSlice99_from_str("TEARDOWN "), SMOLRTSP_METHOD_TEARDOWN));
    CHECK_CALL(assert_ok(CharSlice99_from_str("GET_PARAMETER "), SMOLRTSP_METHOD_GET_PARAMETER));
    CHECK_CALL(assert_ok(CharSlice99_from_str("SET_PARAMETER "), SMOLRTSP_METHOD_SET_PARAMETER));
    CHECK_CALL(assert_ok(CharSlice99_from_str("REDIRECT "), SMOLRTSP_METHOD_REDIRECT));
    CHECK_CALL(assert_ok(CharSlice99_from_str("RECORD "), SMOLRTSP_METHOD_RECORD));

    CHECK_CALL(assert_pending(CharSlice99_from_str("OPTIONS")));

    CHECK_CALL(assert_err(CharSlice99_from_str("~123")));
    CHECK_CALL(assert_err(CharSlice99_from_str("/ hello ~19r world")));

    PASS();
}

SUITE(method) {
    RUN_TEST(parse_method);
}
