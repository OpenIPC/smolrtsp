#ifndef SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H
#define SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/rtsp_version.h>

struct SmolRTSP_RTSPVersionDeserializer;
typedef struct SmolRTSP_RTSPVersionDeserializer SmolRTSP_RTSPVersionDeserializer;

SmolRTSP_RTSPVersionDeserializer *SmolRTSP_RTSPVersionDeserializer_new(void);
void SmolRTSP_RTSPVersionDeserializer_free(SmolRTSP_RTSPVersionDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RTSPVersionDeserializer_deserialize(
    SmolRTSP_RTSPVersionDeserializer *restrict self, SmolRTSP_RTSPVersion *restrict version,
    size_t size, const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H
