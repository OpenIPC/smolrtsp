#include <smolrtsp/types/rtsp_version.h>

#include <greatest.h>

static enum greatest_test_res assert_pending(CharSlice99 input) {
    SmolRTSP_RtspVersion result;
    SmolRTSP_ParseResult res = SmolRTSP_RtspVersion_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    PASS();
}

static enum greatest_test_res assert_ok(CharSlice99 input, SmolRTSP_RtspVersion expected) {
    SmolRTSP_RtspVersion result;
    SmolRTSP_ParseResult res = SmolRTSP_RtspVersion_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(SmolRTSP_RtspVersion_eq(result, expected));
    PASS();
}

static enum greatest_test_res assert_err(CharSlice99 input) {
    SmolRTSP_RtspVersion result;
    SmolRTSP_ParseResult res = SmolRTSP_RtspVersion_parse(&result, input);
    ASSERT(MATCHES(res, SmolRTSP_ParseResult_Failure));
    PASS();
}

SmolRTSP_ParseResult smolrtsp_match_numeric(CharSlice99 input);

TEST parse_rtsp_version(void) {
    const CharSlice99 input = CharSlice99_from_str("RTSP/1.1 ");

    for (size_t i = 0; i < input.len - 1; i++) {
        CHECK_CALL(assert_pending(CharSlice99_update_len(input, i)));
    }

    CHECK_CALL(assert_ok(input, (SmolRTSP_RtspVersion){.major = 1, .minor = 1}));

    CHECK_CALL(assert_err(CharSlice99_from_str("192")));
    CHECK_CALL(assert_err(CharSlice99_from_str(" ~ RTSP/")));

    PASS();
}

TEST serialize_rtsp_version(void) {
    char buffer[20] = {0};

    SmolRTSP_RtspVersion_serialize((SmolRTSP_RtspVersion){1, 0}, smolrtsp_string_writer(buffer));

    ASSERT_EQ(strcmp(buffer, "RTSP/1.0"), 0);

    PASS();
}

SUITE(rtsp_version) {
    RUN_TEST(parse_rtsp_version);
    RUN_TEST(serialize_rtsp_version);
}
