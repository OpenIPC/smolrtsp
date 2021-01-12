#include <smolrtsp/rtsp_version.h>

#include <string.h>

#include "nala.h"

static void assert_pending(Slice99 input) {
    SmolRTSP_RTSPVersion result;
    SmolRTSP_DeserializeResult res = SmolRTSP_RTSPVersion_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(Slice99 input, SmolRTSP_RTSPVersion expected) {
    SmolRTSP_RTSPVersion result;
    SmolRTSP_DeserializeResult res = SmolRTSP_RTSPVersion_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_RTSPVersion_eq(result, expected));
}

TEST(deserialize_rtsp_version) {
    assert_pending(Slice99_from_str(""));
    assert_pending(Slice99_from_str("RT"));
    assert_pending(Slice99_from_str("RTSP/"));
    assert_pending(Slice99_from_str("RTSP/1"));
    assert_pending(Slice99_from_str("RTSP/1."));
    assert_pending(Slice99_from_str("RTSP/1.1"));
    assert_ok(Slice99_from_str("RTSP/1.1 "), (SmolRTSP_RTSPVersion){.major = 1, .minor = 1});
}
