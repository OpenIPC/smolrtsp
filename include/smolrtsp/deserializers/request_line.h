/**
 * @file
 *  An RTSP request line deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_REQUEST_LINE_H
#define SMOLRTSP_DESERIALIZERS_REQUEST_LINE_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/opaque_type.h>
#include <smolrtsp/request_line.h>

/**
 * A deserializer of an RTSP request line.
 */
SMOLRTSP_OPAQUE_TYPE(SmolRTSP_RequestLineDeserializer);

SmolRTSP_RequestLineDeserializer *SmolRTSP_RequestLineDeserializer_new(void);
void SmolRTSP_RequestLineDeserializer_free(SmolRTSP_RequestLineDeserializer *self);

/**
 * Which member of an RTSP request line is being parsed right now by
 * #SmolRTSP_RequestLineDeserializer.
 */
typedef enum {
    SmolRTSP_RequestLineDeserializerStateInProgressMethod,
    SmolRTSP_RequestLineDeserializerStateInProgressRequestURI,
    SmolRTSP_RequestLineDeserializerStateInProgressRTSPVersion,
    SmolRTSP_RequestLineDeserializerStateInProgressCRLF,
    SmolRTSP_RequestLineDeserializerStateInProgressDone,
} SmolRTSP_RequestLineDeserializerStateInProgress;

/**
 * A state in which #SmolRTSP_RequestLineDeserializer is located.
 */
typedef struct {
    SmolRTSP_RequestLineDeserializerStateInProgress in_progress;
    bool is_ok;
} SmolRTSP_RequestLineDeserializerState;

SmolRTSP_RequestLineDeserializerState
SmolRTSP_RequestLineDeserializer_state(const SmolRTSP_RequestLineDeserializer *self);

SmolRTSP_RequestLine SmolRTSP_RequestLineDeserializer_inner(SmolRTSP_RequestLineDeserializer *self);
size_t SmolRTSP_RequestLineDeserializer_bytes_read(SmolRTSP_RequestLineDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_RequestLineDeserializer_deserialize(
    SmolRTSP_RequestLineDeserializer *restrict self, Slice99 *restrict data);

#endif // SMOLRTSP_DESERIALIZERS_REQUEST_LINE_H
