/**
 * @file
 * @brief An RTSP request deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/opaque_type.h>
#include <smolrtsp/request.h>

SMOLRTSP_OPAQUE_TYPE(SmolRTSP_RequestDeserializer);

typedef enum {
    SmolRTSP_RequestDeserializerStateInProgressRequestLine,
    SmolRTSP_RequestDeserializerStateInProgressHeaderMap,
    SmolRTSP_RequestDeserializerStateInProgressMessageBody,
    SmolRTSP_RequestDeserializerStateInProgressDone,
} SmolRTSP_RequestDeserializerStateInProgress;

typedef struct {
    SmolRTSP_RequestDeserializerStateInProgress in_progress;
    bool is_ok;
} SmolRTSP_RequestDeserializerState;

SmolRTSP_RequestDeserializer *
SmolRTSP_RequestDeserializer_new(size_t size, SmolRTSP_Header headers[static size]);
void SmolRTSP_RequestDeserializer_free(SmolRTSP_RequestDeserializer *self);

SmolRTSP_RequestDeserializerState
SmolRTSP_RequestDeserializer_state(const SmolRTSP_RequestDeserializer *self);

SmolRTSP_Request SmolRTSP_RequestDeserializer_inner(SmolRTSP_RequestDeserializer *self);
size_t SmolRTSP_RequestDeserializer_bytes_read(SmolRTSP_RequestDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RequestDeserializer_deserialize(
    SmolRTSP_RequestDeserializer *restrict self, SmolRTSP_Slice *restrict data);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_H
