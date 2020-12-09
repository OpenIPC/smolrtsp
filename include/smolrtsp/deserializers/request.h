#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/request.h>

struct SmolRTSP_RequestDeserializer;
typedef struct SmolRTSP_RequestDeserializer SmolRTSP_RequestDeserializer;

typedef enum {
    SmolRTSP_RequestDeserializerStateStart,
    SmolRTSP_RequestDeserializerStateRequestLineParsed,
    SmolRTSP_RequestDeserializerStateHeaderMapParsed,
    SmolRTSP_RequestDeserializerStateMessageBodyParsed,
    SmolRTSP_RequestDeserializerStateRequestLineErr,
    SmolRTSP_RequestDeserializerStateHeaderMapErr,
    SmolRTSP_RequestDeserializerStateMessageBodyErr,
    SmolRTSP_RequestDeserializerStateContentLengthErr,
} SmolRTSP_RequestDeserializerState;

SmolRTSP_RequestDeserializer *SmolRTSP_RequestDeserializer_new(void);
void SmolRTSP_RequestDeserializer_free(SmolRTSP_RequestDeserializer *self);

SmolRTSP_RequestDeserializerState
SmolRTSP_RequestDeserializer_state(const SmolRTSP_RequestDeserializer *self);

SmolRTSP_Request SmolRTSP_RequestDeserializer_inner(SmolRTSP_RequestDeserializer *self);
size_t SmolRTSP_RequestDeserializer_bytes_read(SmolRTSP_RequestDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RequestDeserializer_deserialize(
    SmolRTSP_RequestDeserializer *restrict self, size_t size,
    const char data[restrict static size]);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_H
