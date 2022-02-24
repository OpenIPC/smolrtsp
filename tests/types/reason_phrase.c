#include <smolrtsp/types/reason_phrase.h>

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_ReasonPhrase)

TEST parse_reason_phrase(void) {
    TEST_PARSE(
        "Moved Temporarily\r\n", CharSlice99_from_str("Moved Temporarily"));

    PASS();
}

SUITE(types_reason_phrase) {
    RUN_TEST(parse_reason_phrase);
}
