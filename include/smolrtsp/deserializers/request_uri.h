/**
 * @file
 * @brief An RTSP request URI deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_URI_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_URI_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/opaque_type.h>
#include <smolrtsp/request_uri.h>

SMOLRTSP_OPAQUE_TYPE(SmolRTSP_RequestURIDeserializer);

SmolRTSP_RequestURIDeserializer *SmolRTSP_RequestURIDeserializer_new(void);
void SmolRTSP_RequestURIDeserializer_free(SmolRTSP_RequestURIDeserializer *self);

SmolRTSP_RequestURI SmolRTSP_RequestURIDeserializer_inner(SmolRTSP_RequestURIDeserializer *self);
size_t SmolRTSP_RequestURIDeserializer_bytes_read(SmolRTSP_RequestURIDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RequestURIDeserializer_deserialize(
    SmolRTSP_RequestURIDeserializer *restrict self, SmolRTSP_Slice *restrict data);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_URI_H
