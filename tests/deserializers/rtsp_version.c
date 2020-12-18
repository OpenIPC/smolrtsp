#include <smolrtsp/deserializers/rtsp_version.h>

#include <string.h>

#include "../nala.h"

static void check(const char *version, SmolRTSP_RTSPVersion expected) {
    SmolRTSP_RTSPVersionDeserializer *deser = SmolRTSP_RTSPVersionDeserializer_new();
    ASSERT_NE(deser, NULL);

    SmolRTSP_Slice data = SmolRTSP_Slice_from_str(version);
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_RTSPVersionDeserializer_deserialize(deser, &data);
    const SmolRTSP_RTSPVersion inner = SmolRTSP_RTSPVersionDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_RTSPVersionDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(version));
    ASSERT(SmolRTSP_RTSPVersion_eq(&inner, &expected));

    SmolRTSP_RTSPVersionDeserializer_free(deser);
}

TEST(test_deserializers_rtsp_version) {
    check("RTSP/1.1", (SmolRTSP_RTSPVersion){.major = 1, .minor = 1});
    check("    RTSP/1.1", (SmolRTSP_RTSPVersion){.major = 1, .minor = 1});
}
