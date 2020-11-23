#ifndef SMOLRTSP_DESERIALIZERS_HEADER_MAP_H
#define SMOLRTSP_DESERIALIZERS_HEADER_MAP_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/header_map.h>

struct SmolRTSP_HeaderMapDeserializer;
typedef struct SmolRTSP_HeaderMapDeserializer SmolRTSP_HeaderMapDeserializer;

SmolRTSP_HeaderMapDeserializer *SmolRTSP_HeaderMapDeserializer_new(void);
void SmolRTSP_HeaderMapDeserializer_free(SmolRTSP_HeaderMapDeserializer *deserializer);

SmolRTSP_DeserializeResult SmolRTSP_HeaderMapDeserializer_deserialize(
    SmolRTSP_HeaderMapDeserializer *restrict deserializer, SmolRTSP_HeaderMap *restrict map,
    size_t size, const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_HEADER_MAP_H
