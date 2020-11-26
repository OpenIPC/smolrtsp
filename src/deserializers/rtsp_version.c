#include "../parsing_aux.h"
#include <smolrtsp/deserializers/rtsp_version.h>
#include <smolrtsp/limits.h>

#include <inttypes.h>

struct SmolRTSP_RTSPVersionDeserializer {
    SmolRTSP_RTSPVersion inner;
    size_t bytes_read;
};

SmolRTSP_RTSPVersionDeserializer *SmolRTSP_RTSPVersionDeserializer_new(void) {
    SmolRTSP_RTSPVersionDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;

    return self;
}

void SmolRTSP_RTSPVersionDeserializer_free(SmolRTSP_RTSPVersionDeserializer *self) {
    free(self);
}

SmolRTSP_RTSPVersion
SmolRTSP_RTSPVersionDeserializer_inner(SmolRTSP_RTSPVersionDeserializer *self) {
    return self->inner;
}

size_t SmolRTSP_RTSPVersionDeserializer_bytes_read(SmolRTSP_RTSPVersionDeserializer *self) {
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RTSPVersionDeserializer_deserialize(
    SmolRTSP_RTSPVersionDeserializer *restrict self, size_t size, const void *restrict data) {
    SmolRTSP_RTSPVersion version;
    SmolRTSP_DeserializeResult res = SmolRTSP_parse(
        SMOLRTSP_RTSP_VERSION_SIZE, size, data, "RTSP/%" SCNuLEAST8 ".%" SCNuLEAST8 "%n", 3,
        &version.major, &version.minor, &self->bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        self->inner = version;
    }

    return res;
}
