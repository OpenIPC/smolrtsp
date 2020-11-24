#ifndef SMOLRTSP_DESERIALIZERS_METHOD_H
#define SMOLRTSP_DESERIALIZERS_METHOD_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/method.h>

struct SmolRTSP_MethodDeserializer;
typedef struct SmolRTSP_MethodDeserializer SmolRTSP_MethodDeserializer;

SmolRTSP_MethodDeserializer *SmolRTSP_MethodDeserializer_new(void);
void SmolRTSP_MethodDeserializer_free(SmolRTSP_MethodDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_MethodDeserializer_deserialize(
    SmolRTSP_MethodDeserializer *restrict self, SmolRTSP_Method *restrict method, size_t size,
    const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_METHOD_H
