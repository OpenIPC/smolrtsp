/**
 * @file
 * @brief An RTSP method deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_METHOD_H
#define SMOLRTSP_DESERIALIZERS_METHOD_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/method.h>

struct SmolRTSP_MethodDeserializer;
typedef struct SmolRTSP_MethodDeserializer SmolRTSP_MethodDeserializer;

SmolRTSP_MethodDeserializer *SmolRTSP_MethodDeserializer_new(void);
void SmolRTSP_MethodDeserializer_free(SmolRTSP_MethodDeserializer *self);

SmolRTSP_Method SmolRTSP_MethodDeserializer_inner(SmolRTSP_MethodDeserializer *self);
size_t SmolRTSP_MethodDeserializer_bytes_read(SmolRTSP_MethodDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_MethodDeserializer_deserialize(
    SmolRTSP_MethodDeserializer *restrict self, size_t size, const char data[restrict]);

#endif // SMOLRTSP_DESERIALIZERS_METHOD_H
