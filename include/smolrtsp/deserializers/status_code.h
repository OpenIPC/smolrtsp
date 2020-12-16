/**
 * @file
 * @brief An RTSP status code deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_STATUS_CODE_H
#define SMOLRTSP_DESERIALIZERS_STATUS_CODE_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/opaque_type.h>
#include <smolrtsp/status_code.h>

SMOLRTSP_OPAQUE_TYPE(SmolRTSP_StatusCodeDeserializer);

SmolRTSP_StatusCodeDeserializer *SmolRTSP_StatusCodeDeserializer_new(void);
void SmolRTSP_StatusCodeDeserializer_free(SmolRTSP_StatusCodeDeserializer *self);

SmolRTSP_StatusCode SmolRTSP_StatusCodeDeserializer_inner(SmolRTSP_StatusCodeDeserializer *self);
size_t SmolRTSP_StatusCodeDeserializer_bytes_read(SmolRTSP_StatusCodeDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_StatusCodeDeserializer_deserialize(
    SmolRTSP_StatusCodeDeserializer *restrict self, SmolRTSP_Slice data);

#endif // SMOLRTSP_DESERIALIZERS_STATUS_CODE_H
