#include <smolrtsp/types/header.h>

#include "test_util.h"
#include <greatest.h>

DEF_TEST_PARSE(SmolRTSP_Header)

TEST parse_header(void) {
    TEST_PARSE(
        "User-Agent: LibVLC/3.0.8 (LIVE555 Streaming Media v2018.02.18)\r\n",
        ((SmolRTSP_Header){
            SMOLRTSP_HEADER_USER_AGENT,
            CharSlice99_from_str(
                "LibVLC/3.0.8 (LIVE555 Streaming Media v2018.02.18)"),
        }));

    SmolRTSP_Header result;

    ASSERT(SmolRTSP_ParseResult_is_failure(
        SmolRTSP_Header_parse(&result, CharSlice99_from_str("~@~"))));

    PASS();
}

TEST serialize_header(void) {
    char buffer[200] = {0};

    const SmolRTSP_Header header = {
        SMOLRTSP_HEADER_CONTENT_LENGTH,
        CharSlice99_from_str("123"),
    };

    const ssize_t ret =
        SmolRTSP_Header_serialize(&header, smolrtsp_string_writer(buffer));

    const char *expected = "Content-Length: 123\r\n";

    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_STR_EQ(expected, buffer);

    PASS();
}

SUITE(types_header) {
    RUN_TEST(parse_header);
    RUN_TEST(serialize_header);
}
