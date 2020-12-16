/**
 * @file
 * @brief An RTSP message body deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_MESSAGE_BODY_H
#define SMOLRTSP_DESERIALIZERS_MESSAGE_BODY_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/opaque_type.h>

SMOLRTSP_OPAQUE_TYPE(SmolRTSP_MessageBodyDeserializer);

SmolRTSP_MessageBodyDeserializer *SmolRTSP_MessageBodyDeserializer_new(size_t content_length);
void SmolRTSP_MessageBodyDeserializer_free(SmolRTSP_MessageBodyDeserializer *self);

SmolRTSP_MessageBody SmolRTSP_MessageBodyDeserializer_inner(SmolRTSP_MessageBodyDeserializer *self);
size_t SmolRTSP_MessageBodyDeserializer_bytes_read(SmolRTSP_MessageBodyDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_MessageBodyDeserializer_deserialize(
    SmolRTSP_MessageBodyDeserializer *restrict self, SmolRTSP_Slice data);

#endif // SMOLRTSP_DESERIALIZERS_MESSAGE_BODY_H
