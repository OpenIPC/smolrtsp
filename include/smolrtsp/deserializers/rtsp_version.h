#ifndef SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H
#define SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/rtsp_version.h>

struct SmolRTSP_RTSPVersionDeserializer;
typedef struct SmolRTSP_RTSPVersionDeserializer SmolRTSP_RTSPVersionDeserializer;

SmolRTSP_RTSPVersionDeserializer *SmolRTSP_RTSPVersionDeserializer_new(void);
void SmolRTSP_RTSPVersionDeserializer_free(SmolRTSP_RTSPVersionDeserializer *self);

SmolRTSP_RTSPVersion SmolRTSP_RTSPVersionDeserializer_inner(SmolRTSP_RTSPVersionDeserializer *self);
size_t SmolRTSP_RTSPVersionDeserializer_bytes_read(SmolRTSP_RTSPVersionDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RTSPVersionDeserializer_deserialize(
    SmolRTSP_RTSPVersionDeserializer *restrict self, size_t size, const char data[restrict]);

#endif // SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H
