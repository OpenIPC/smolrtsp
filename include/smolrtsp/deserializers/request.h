#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_H

#include <smolrtsp/request.h>

struct SmolRTSP_RequestDeserializer;
typedef struct SmolRTSP_RequestDeserializer SmolRTSP_RequestDeserializer;

typedef enum {
    SmolRTSP_RequestDeserializerStateNothingParsed,
    SmolRTSP_RequestDeserializerStateMethodParsed,
    SmolRTSP_RequestDeserializerStateRequestURIParsed,
    SmolRTSP_RequestDeserializerStateRTSPVersionParsed,
    SmolRTSP_RequestDeserializerStateHeaderParsed,
    SmolRTSP_RequestDeserializerStateHeaderMapParsed,
    SmolRTSP_RequestDeserializerStateMessageBodyParsed,
    SmolRTSP_RequestDeserializerStateErr,
} SmolRTSP_RequestDeserializerState;

SmolRTSP_RequestDeserializer *SmolRTSP_RequestDeserializer_new(void);
void SmolRTSP_RequestDeserializer_free(SmolRTSP_RequestDeserializer *self);

SmolRTSP_RequestDeserializerState
SmolRTSP_RequestDeserializer_state(const SmolRTSP_RequestDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RequestDeserializer_deserialize(
    SmolRTSP_RequestDeserializer *restrict self, SmolRTSP_Request *restrict request, size_t size,
    const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_H
