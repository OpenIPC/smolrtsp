#include <smolrtsp/types/status_code.h>

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_StatusCode)

TEST parse_status_code(void) {
    TEST_PARSE("100 ", SMOLRTSP_STATUS_CONTINUE);
    TEST_PARSE("200 ", SMOLRTSP_STATUS_OK);
    TEST_PARSE("303 ", SMOLRTSP_STATUS_SEE_OTHER);
    TEST_PARSE("404 ", SMOLRTSP_STATUS_NOT_FOUND);
    TEST_PARSE("551 ", SMOLRTSP_STATUS_OPTION_NOT_SUPPORTED);

    SmolRTSP_StatusCode result;

    ASSERT(SmolRTSP_ParseResult_is_failure(
        SmolRTSP_StatusCode_parse(&result, CharSlice99_from_str("blah"))));
    ASSERT(SmolRTSP_ParseResult_is_failure(SmolRTSP_StatusCode_parse(
        &result, CharSlice99_from_str("~ 2424 blah"))));

    PASS();
}

TEST serialize_status_code(void) {
    char buffer[20] = {0};

    const SmolRTSP_StatusCode status = SMOLRTSP_STATUS_NOT_FOUND;

    const ssize_t ret =
        SmolRTSP_StatusCode_serialize(&status, smolrtsp_string_writer(buffer));

    const char *expected = "404";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(types_status_code) {
    RUN_TEST(parse_status_code);
    RUN_TEST(serialize_status_code);
}
