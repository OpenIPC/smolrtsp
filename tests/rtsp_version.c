#include <smolrtsp/rtsp_version.h>

#include <string.h>

#include "nala.h"

static void check(Slice99 version, SmolRTSP_RTSPVersion expected) {
    SmolRTSP_RTSPVersion result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_RTSPVersion_deserialize(&result, &version);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_RTSPVersion_eq(result, expected));
}

TEST(deserialize_rtsp_version) {
    check(Slice99_from_str("RTSP/1.1"), (SmolRTSP_RTSPVersion){.major = 1, .minor = 1});
    check(Slice99_from_str("    RTSP/1.1"), (SmolRTSP_RTSPVersion){.major = 1, .minor = 1});
}
