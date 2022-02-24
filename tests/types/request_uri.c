#include <smolrtsp/types/request_uri.h>

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_RequestUri)

TEST parse_request_uri(void) {
    TEST_PARSE(
        "http://example.com ", CharSlice99_from_str("http://example.com"));

    PASS();
}

SUITE(types_request_uri) {
    RUN_TEST(parse_request_uri);
}
