/**
 * @file
 *  An RTSP version deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H
#define SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/opaque_type.h>
#include <smolrtsp/rtsp_version.h>

/**
 * A deserializer of an RTSP version.
 */
SMOLRTSP_OPAQUE_TYPE(SmolRTSP_RTSPVersionDeserializer);

SmolRTSP_RTSPVersionDeserializer *SmolRTSP_RTSPVersionDeserializer_new(void);
void SmolRTSP_RTSPVersionDeserializer_free(SmolRTSP_RTSPVersionDeserializer *self);

SmolRTSP_RTSPVersion SmolRTSP_RTSPVersionDeserializer_inner(SmolRTSP_RTSPVersionDeserializer *self);
size_t SmolRTSP_RTSPVersionDeserializer_bytes_read(SmolRTSP_RTSPVersionDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RTSPVersionDeserializer_deserialize(
    SmolRTSP_RTSPVersionDeserializer *restrict self, Slice99 *restrict data);

#endif // SMOLRTSP_DESERIALIZERS_RTSP_VERSION_H
