#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_LINE_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_LINE_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/request_line.h>

struct SmolRTSP_RequestLineDeserializer;
typedef struct SmolRTSP_RequestLineDeserializer SmolRTSP_RequestLineDeserializer;

SmolRTSP_RequestLineDeserializer *SmolRTSP_RequestLineDeserializer_new(void);
void SmolRTSP_RequestLineDeserializer_free(SmolRTSP_RequestLineDeserializer *self);

typedef enum {
    SmolRTSP_RequestLineDeserializerStateStart,
    SmolRTSP_RequestLineDeserializerStateMethodParsed,
    SmolRTSP_RequestLineDeserializerStateRequestURIParsed,
    SmolRTSP_RequestLineDeserializerStateRTSPVersionParsed,
    SmolRTSP_RequestLineDeserializerStateErr,
} SmolRTSP_RequestLineDeserializerState;

SmolRTSP_RequestLineDeserializerState
SmolRTSP_RequestLineDeserializer_state(const SmolRTSP_RequestLineDeserializer *self);

SmolRTSP_RequestLine SmolRTSP_RequestLineDeserializer_inner(SmolRTSP_RequestLineDeserializer *self);
size_t SmolRTSP_RequestLineDeserializer_bytes_read(SmolRTSP_RequestLineDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RequestLineDeserializer_deserialize(
    SmolRTSP_RequestLineDeserializer *restrict self, size_t size, const void *restrict data);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_LINE_H
