#include <smolrtsp/method.h>

#include "nala.h"

static void assert_pending(Slice99 input) {
    SmolRTSP_Method result;
    SmolRTSP_DeserializeResult res = SmolRTSP_Method_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(Slice99 input, SmolRTSP_Method expected) {
    SmolRTSP_Method result;
    SmolRTSP_DeserializeResult res = SmolRTSP_Method_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

static void assert_err(Slice99 input) {
    SmolRTSP_Method result;
    SmolRTSP_DeserializeResult res = SmolRTSP_Method_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultErr);
}

TEST(deserialize_method) {
    assert_ok(Slice99_from_str("OPTIONS "), SMOLRTSP_METHOD_OPTIONS);
    assert_ok(Slice99_from_str("DESCRIBE "), SMOLRTSP_METHOD_DESCRIBE);
    assert_ok(Slice99_from_str("ANNOUNCE "), SMOLRTSP_METHOD_ANNOUNCE);
    assert_ok(Slice99_from_str("SETUP "), SMOLRTSP_METHOD_SETUP);
    assert_ok(Slice99_from_str("PLAY "), SMOLRTSP_METHOD_PLAY);
    assert_ok(Slice99_from_str("PAUSE "), SMOLRTSP_METHOD_PAUSE);
    assert_ok(Slice99_from_str("TEARDOWN "), SMOLRTSP_METHOD_TEARDOWN);
    assert_ok(Slice99_from_str("GET_PARAMETER "), SMOLRTSP_METHOD_GET_PARAMETER);
    assert_ok(Slice99_from_str("SET_PARAMETER "), SMOLRTSP_METHOD_SET_PARAMETER);
    assert_ok(Slice99_from_str("REDIRECT "), SMOLRTSP_METHOD_REDIRECT);
    assert_ok(Slice99_from_str("RECORD "), SMOLRTSP_METHOD_RECORD);

    assert_pending(Slice99_from_str("OPTIONS"));

    assert_err(Slice99_from_str("~123"));
    assert_err(Slice99_from_str("/ hello ~19r world"));
}
