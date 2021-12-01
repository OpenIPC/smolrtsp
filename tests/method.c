#include <smolrtsp/method.h>

#include "nala/nala.h"

static void assert_pending(CharSlice99 input) {
    SmolRTSP_Method result;
    SmolRTSP_ParseResult res = SmolRTSP_Method_parse(&result, &input);
    ASSERT_EQ(res, SmolRTSP_ParseResult_Pending);
}

static void assert_ok(CharSlice99 input, SmolRTSP_Method expected) {
    SmolRTSP_Method result;
    SmolRTSP_ParseResult res = SmolRTSP_Method_parse(&result, &input);
    ASSERT_EQ(res, SmolRTSP_ParseResult_Ok);
    ASSERT(CharSlice99_primitive_eq(result, expected));
}

static void assert_err(CharSlice99 input) {
    SmolRTSP_Method result;
    SmolRTSP_ParseResult res = SmolRTSP_Method_parse(&result, &input);
    ASSERT_EQ(res, SmolRTSP_ParseResult_Err);
}

TEST(parse_method) {
    assert_ok(CharSlice99_from_str("OPTIONS "), SMOLRTSP_METHOD_OPTIONS);
    assert_ok(CharSlice99_from_str("DESCRIBE "), SMOLRTSP_METHOD_DESCRIBE);
    assert_ok(CharSlice99_from_str("ANNOUNCE "), SMOLRTSP_METHOD_ANNOUNCE);
    assert_ok(CharSlice99_from_str("SETUP "), SMOLRTSP_METHOD_SETUP);
    assert_ok(CharSlice99_from_str("PLAY "), SMOLRTSP_METHOD_PLAY);
    assert_ok(CharSlice99_from_str("PAUSE "), SMOLRTSP_METHOD_PAUSE);
    assert_ok(CharSlice99_from_str("TEARDOWN "), SMOLRTSP_METHOD_TEARDOWN);
    assert_ok(CharSlice99_from_str("GET_PARAMETER "), SMOLRTSP_METHOD_GET_PARAMETER);
    assert_ok(CharSlice99_from_str("SET_PARAMETER "), SMOLRTSP_METHOD_SET_PARAMETER);
    assert_ok(CharSlice99_from_str("REDIRECT "), SMOLRTSP_METHOD_REDIRECT);
    assert_ok(CharSlice99_from_str("RECORD "), SMOLRTSP_METHOD_RECORD);

    assert_pending(CharSlice99_from_str("OPTIONS"));

    assert_err(CharSlice99_from_str("~123"));
    assert_err(CharSlice99_from_str("/ hello ~19r world"));
}
