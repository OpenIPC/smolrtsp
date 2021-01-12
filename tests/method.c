#include <smolrtsp/method.h>

#include <string.h>

#include "nala.h"

static void check(Slice99 method, SmolRTSP_Method expected) {
    SmolRTSP_Method result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_Method_deserialize(&result, &method);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(deserialize_method) {
    check(Slice99_from_str("OPTIONS "), SMOLRTSP_METHOD_OPTIONS);
    check(Slice99_from_str("DESCRIBE "), SMOLRTSP_METHOD_DESCRIBE);
    check(Slice99_from_str("ANNOUNCE "), SMOLRTSP_METHOD_ANNOUNCE);
    check(Slice99_from_str("SETUP "), SMOLRTSP_METHOD_SETUP);
    check(Slice99_from_str("PLAY "), SMOLRTSP_METHOD_PLAY);
    check(Slice99_from_str("PAUSE "), SMOLRTSP_METHOD_PAUSE);
    check(Slice99_from_str("TEARDOWN "), SMOLRTSP_METHOD_TEARDOWN);
    check(Slice99_from_str("GET_PARAMETER "), SMOLRTSP_METHOD_GET_PARAMETER);
    check(Slice99_from_str("SET_PARAMETER "), SMOLRTSP_METHOD_SET_PARAMETER);
    check(Slice99_from_str("REDIRECT "), SMOLRTSP_METHOD_REDIRECT);
    check(Slice99_from_str("RECORD "), SMOLRTSP_METHOD_RECORD);
}
