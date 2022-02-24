#include <smolrtsp/types/method.h>

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_Method)

TEST parse_method(void) {
    TEST_PARSE("OPTIONS ", SMOLRTSP_METHOD_OPTIONS);
    TEST_PARSE("DESCRIBE ", SMOLRTSP_METHOD_DESCRIBE);
    TEST_PARSE("ANNOUNCE ", SMOLRTSP_METHOD_ANNOUNCE);
    TEST_PARSE("SETUP ", SMOLRTSP_METHOD_SETUP);
    TEST_PARSE("PLAY ", SMOLRTSP_METHOD_PLAY);
    TEST_PARSE("PAUSE ", SMOLRTSP_METHOD_PAUSE);
    TEST_PARSE("TEARDOWN ", SMOLRTSP_METHOD_TEARDOWN);
    TEST_PARSE("GET_PARAMETER ", SMOLRTSP_METHOD_GET_PARAMETER);
    TEST_PARSE("SET_PARAMETER ", SMOLRTSP_METHOD_SET_PARAMETER);
    TEST_PARSE("REDIRECT ", SMOLRTSP_METHOD_REDIRECT);
    TEST_PARSE("RECORD ", SMOLRTSP_METHOD_RECORD);

    SmolRTSP_Method result;

    ASSERT(SmolRTSP_ParseResult_is_failure(
        SmolRTSP_Method_parse(&result, CharSlice99_from_str("~123"))));
    ASSERT(SmolRTSP_ParseResult_is_failure(SmolRTSP_Method_parse(
        &result, CharSlice99_from_str("/ hello ~19r world"))));

    PASS();
}

SUITE(types_method) {
    RUN_TEST(parse_method);
}
