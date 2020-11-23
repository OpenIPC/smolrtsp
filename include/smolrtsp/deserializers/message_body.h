#ifndef SMOLRTSP_DESERIALIZERS_MESSAGE_BODY_H
#define SMOLRTSP_DESERIALIZERS_MESSAGE_BODY_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/message_body.h>

struct SmolRTSP_MessageBodyDeserializer;
typedef struct SmolRTSP_MessageBodyDeserializer SmolRTSP_MessageBodyDeserializer;

SmolRTSP_MessageBodyDeserializer *SmolRTSP_MessageBodyDeserializer_new(size_t content_length);
void SmolRTSP_MessageBodyDeserializer_free(SmolRTSP_MessageBodyDeserializer *deserializer);

SmolRTSP_DeserializeResult SmolRTSP_MessageBodyDeserializer_deserialize(
    SmolRTSP_MessageBodyDeserializer *restrict deserializer, SmolRTSP_MessageBody *restrict body,
    size_t size, const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_MESSAGE_BODY_H
