#include "../parsing_aux.h"
#include <smolrtsp/deserializers/rtsp_version.h>
#include <smolrtsp/limits.h>

#include <inttypes.h>

struct SmolRTSP_RTSPVersionDeserializer {
    char nothing;
} hollow_deserializer;

SmolRTSP_RTSPVersionDeserializer *SmolRTSP_RTSPVersionDeserializer_new(void) {
    return &hollow_deserializer;
}

void SmolRTSP_RTSPVersionDeserializer_free(SmolRTSP_RTSPVersionDeserializer *self) {}

SmolRTSP_DeserializeResult SmolRTSP_RTSPVersionDeserializer_deserialize(
    SmolRTSP_RTSPVersionDeserializer *restrict self, SmolRTSP_RTSPVersion *restrict version,
    size_t size, const void *restrict data, size_t *restrict bytes_read) {
    SmolRTSP_RTSPVersion parsed_version;
    SmolRTSP_DeserializeResult res = SmolRTSP_parse(
        SMOLRTSP_RTSP_VERSION_SIZE, size, data, "RTSP/%" SCNuLEAST8 ".%" SCNuLEAST8 "%n", 3,
        &parsed_version.major, &parsed_version.minor, bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        *version = parsed_version;
    }

    return res;
}
