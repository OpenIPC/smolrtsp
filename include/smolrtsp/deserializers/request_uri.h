#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_URI_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_URI_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/request_uri.h>

struct SmolRTSP_RequestURIDeserializer;
typedef struct SmolRTSP_RequestURIDeserializer SmolRTSP_RequestURIDeserializer;

SmolRTSP_RequestURIDeserializer *SmolRTSP_RequestURIDeserializer_new(void);
void SmolRTSP_RequestURIDeserializer_free(SmolRTSP_RequestURIDeserializer *deserializer);

SmolRTSP_DeserializeResult SmolRTSP_RequestURIDeserializer_deserialize(
    SmolRTSP_RequestURIDeserializer *restrict deserializer, SmolRTSP_RequestURI *restrict uri,
    size_t size, const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_URI_H
