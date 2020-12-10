#ifndef SMOLRTSP_DESERIALIZERS_CRLF_H
#define SMOLRTSP_DESERIALIZERS_CRLF_H

#include <smolrtsp/crlf.h>
#include <smolrtsp/deserialization.h>

struct SmolRTSP_CRLFDeserializer;
typedef struct SmolRTSP_CRLFDeserializer SmolRTSP_CRLFDeserializer;

SmolRTSP_CRLFDeserializer *SmolRTSP_CRLFDeserializer_new(void);
void SmolRTSP_CRLFDeserializer_free(SmolRTSP_CRLFDeserializer *self);

SmolRTSP_CRLF SmolRTSP_CRLFDeserializer_inner(SmolRTSP_CRLFDeserializer *self);
size_t SmolRTSP_CRLFDeserializer_bytes_read(SmolRTSP_CRLFDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_CRLFDeserializer_deserialize(
    SmolRTSP_CRLFDeserializer *restrict self, size_t size, const char data[restrict]);

#endif // SMOLRTSP_DESERIALIZERS_CRLF_H
