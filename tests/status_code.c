#include <smolrtsp/types/status_code.h>

#include <greatest.h>

static enum greatest_test_res assert_pending(CharSlice99 input) {
    SmolRTSP_StatusCode result;
    SmolRTSP_ParseResult res = SmolRTSP_StatusCode_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    PASS();
}

static enum greatest_test_res assert_ok(CharSlice99 input, SmolRTSP_StatusCode expected) {
    SmolRTSP_StatusCode result;
    SmolRTSP_ParseResult res = SmolRTSP_StatusCode_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(result == expected);
    PASS();
}

static enum greatest_test_res assert_err(CharSlice99 input) {
    SmolRTSP_StatusCode result;
    SmolRTSP_ParseResult res = SmolRTSP_StatusCode_parse(&result, input);
    ASSERT(MATCHES(res, SmolRTSP_ParseResult_Failure));
    PASS();
}

TEST parse_status_code(void) {
    const CharSlice99 input = CharSlice99_from_str("404 ");

    for (size_t i = 0; i < input.len - 1; i++) {
        CHECK_CALL(assert_pending(CharSlice99_update_len(input, i)));
    }

    CHECK_CALL(assert_ok(input, SMOLRTSP_STATUS_CODE_NOT_FOUND));

    CHECK_CALL(assert_err(CharSlice99_from_str("blah")));
    CHECK_CALL(assert_err(CharSlice99_from_str("~ 2424 blah")));

    PASS();
}

TEST serialize_status_code(void) {
    char buffer[20] = {0};

    SmolRTSP_StatusCode_serialize(SMOLRTSP_STATUS_CODE_NOT_FOUND, smolrtsp_strcat_writer(buffer));

    ASSERT_EQ(strcmp(buffer, "404"), 0);

    PASS();
}

SUITE(status_code) {
    RUN_TEST(parse_status_code);
    RUN_TEST(serialize_status_code);
}
