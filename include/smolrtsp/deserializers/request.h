/**
 * @file
 *  An RTSP request deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/opaque_type.h>
#include <smolrtsp/request.h>

/**
 * A deserializer of an RTSP request.
 */
SMOLRTSP_OPAQUE_TYPE(SmolRTSP_RequestDeserializer);

/**
 * Which member of an RTSP request is being parsed right now by #SmolRTSP_RequestDeserializer.
 */
typedef enum {
    SmolRTSP_RequestDeserializerStateInProgressRequestLine,
    SmolRTSP_RequestDeserializerStateInProgressHeaderMap,
    SmolRTSP_RequestDeserializerStateInProgressMessageBody,
    SmolRTSP_RequestDeserializerStateInProgressDone,
} SmolRTSP_RequestDeserializerStateInProgress;

/**
 * A state in which #SmolRTSP_RequestDeserializer is located.
 */
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
    SmolRTSP_RequestDeserializer *restrict self, Slice99 *restrict data);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_H
