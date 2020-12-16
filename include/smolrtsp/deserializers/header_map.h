/**
 * @file
 * @brief An RTSP header map deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_HEADER_MAP_H
#define SMOLRTSP_DESERIALIZERS_HEADER_MAP_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/header_map.h>
#include <smolrtsp/opaque_type.h>

SMOLRTSP_OPAQUE_TYPE(SmolRTSP_HeaderMapDeserializer);

SmolRTSP_HeaderMapDeserializer *
SmolRTSP_HeaderMapDeserializer_new(size_t size, SmolRTSP_Header headers[static size]);
void SmolRTSP_HeaderMapDeserializer_free(SmolRTSP_HeaderMapDeserializer *self);

SmolRTSP_HeaderMap SmolRTSP_HeaderMapDeserializer_inner(SmolRTSP_HeaderMapDeserializer *self);
size_t SmolRTSP_HeaderMapDeserializer_bytes_read(SmolRTSP_HeaderMapDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_HeaderMapDeserializer_deserialize(
    SmolRTSP_HeaderMapDeserializer *restrict self, SmolRTSP_Slice data);

#endif // SMOLRTSP_DESERIALIZERS_HEADER_MAP_H
