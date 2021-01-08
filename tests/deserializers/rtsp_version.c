#include <smolrtsp/rtsp_version.h>

#include <string.h>

#include "../nala.h"

static void check(const char *version, SmolRTSP_RTSPVersion expected) {
    Slice99 data = Slice99_from_str((char *)version);
    SmolRTSP_RTSPVersion result;
    size_t bytes_read = 0;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_RTSPVersion_deserialize(&result, &data, &bytes_read);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(version));
    ASSERT(SmolRTSP_RTSPVersion_eq(&result, &expected));
}

TEST(test_deserializers_rtsp_version) {
    check("RTSP/1.1", (SmolRTSP_RTSPVersion){.major = 1, .minor = 1});
    check("    RTSP/1.1", (SmolRTSP_RTSPVersion){.major = 1, .minor = 1});
}
