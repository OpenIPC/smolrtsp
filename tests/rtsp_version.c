#include <smolrtsp/rtsp_version.h>

#include "nala/nala.h"

static void assert_pending(CharSlice99 input) {
    SmolRTSP_RTSPVersion result;
    SmolRTSP_DeserializeResult res = SmolRTSP_RTSPVersion_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(CharSlice99 input, SmolRTSP_RTSPVersion expected) {
    SmolRTSP_RTSPVersion result;
    SmolRTSP_DeserializeResult res = SmolRTSP_RTSPVersion_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_RTSPVersion_eq(result, expected));
}

static void assert_err(CharSlice99 input) {
    SmolRTSP_RTSPVersion result;
    SmolRTSP_DeserializeResult res = SmolRTSP_RTSPVersion_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultErr);
}

TEST(deserialize_rtsp_version) {
    const CharSlice99 input = CharSlice99_from_str("RTSP/1.1 ");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(CharSlice99_update_len(input, i));
    }

    assert_ok(input, (SmolRTSP_RTSPVersion){.major = 1, .minor = 1});

    assert_err(CharSlice99_from_str("192"));
    assert_err(CharSlice99_from_str(" ~ RTSP/"));
}

TEST(serialize_rtsp_version) {
    char buffer[20] = {0};

    SmolRTSP_RTSPVersion_serialize(
        SmolRTSP_RTSPVersion_new(1, 0), smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(strcmp(buffer, "RTSP/1.0"), 0);
}
