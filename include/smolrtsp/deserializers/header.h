#ifndef SMOLRTSP_DESERIALIZERS_HEADER_H
#define SMOLRTSP_DESERIALIZERS_HEADER_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/header.h>

struct SmolRTSP_HeaderDeserializer;
typedef struct SmolRTSP_HeaderDeserializer SmolRTSP_HeaderDeserializer;

SmolRTSP_HeaderDeserializer *SmolRTSP_HeaderDeserializer_new(void);
void SmolRTSP_HeaderDeserializer_free(SmolRTSP_HeaderDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_HeaderDeserializer_deserialize(
    SmolRTSP_HeaderDeserializer *restrict self, SmolRTSP_Header *restrict header, size_t size,
    const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_HEADER_H
