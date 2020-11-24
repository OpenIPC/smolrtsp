#ifndef SMOLRTSP_DESERIALIZERS_STATUS_CODE_H
#define SMOLRTSP_DESERIALIZERS_STATUS_CODE_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/status_code.h>

struct SmolRTSP_StatusCodeDeserializer;
typedef struct SmolRTSP_StatusCodeDeserializer SmolRTSP_StatusCodeDeserializer;

SmolRTSP_StatusCodeDeserializer *SmolRTSP_StatusCodeDeserializer_new(void);
void SmolRTSP_StatusCodeDeserializer_free(SmolRTSP_StatusCodeDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_StatusCodeDeserializer_deserialize(
    SmolRTSP_StatusCodeDeserializer *restrict self, SmolRTSP_StatusCode *restrict code, size_t size,
    const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_STATUS_CODE_H
