#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_URI_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_URI_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/request_uri.h>

struct SmolRTSP_RequestURIDeserializer;
typedef struct SmolRTSP_RequestURIDeserializer SmolRTSP_RequestURIDeserializer;

SmolRTSP_RequestURIDeserializer *SmolRTSP_RequestURIDeserializer_new(void);
void SmolRTSP_RequestURIDeserializer_free(SmolRTSP_RequestURIDeserializer *self);

SmolRTSP_RequestURI SmolRTSP_RequestURIDeserializer_inner(SmolRTSP_RequestURIDeserializer *self);
size_t SmolRTSP_RequestURIDeserializer_bytes_read(SmolRTSP_RequestURIDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RequestURIDeserializer_deserialize(
    SmolRTSP_RequestURIDeserializer *restrict self, size_t size, const char data[restrict]);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_URI_H
