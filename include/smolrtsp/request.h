/**
 * @file
 *  An RTSP request.
 */

#ifndef SMOLRTSP_REQUEST_H
#define SMOLRTSP_REQUEST_H

#include <smolrtsp/header_map.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/request_line.h>

#include <stdbool.h>

/**
 * The start state of #SmolRTSP_RequestDeserializerState.
 */
#define SMOLRTSP_REQUEST_DESERIALIZER_STATE_INIT                                                   \
    (SmolRTSP_RequestDeserializerState) {                                                          \
        .start_line = SMOLRTSP_REQUEST_LINE_DESERIALIZER_STATE_INIT,                               \
        .tag = SmolRTSP_RequestDeserializerStateRequestLine,                                       \
    }

/**
 * An RTSP request.
 */
typedef struct {
    /**
     * The request line.
     */
    SmolRTSP_RequestLine start_line;

    /**
     * The header map.
     */
    SmolRTSP_HeaderMap header_map;

    /**
     * The message body.
     */
    SmolRTSP_MessageBody body;
} SmolRTSP_Request;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] self The instance to be serialized.
 * @param[in] user_writer The function to be provided with serialized data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Request_serialize(
    SmolRTSP_Request self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state of deserialization of #SmolRTSP_Request.
 */
typedef struct {
    /**
     * The state of a request line being deserialized.
     */
    SmolRTSP_RequestLineDeserializerState start_line;

    /**
     * What part of a request is being deserialized right now.
     */
    enum {
        SmolRTSP_RequestDeserializerStateRequestLine,
        SmolRTSP_RequestDeserializerStateHeaderMap,
        SmolRTSP_RequestDeserializerStateMessageBody,
        SmolRTSP_RequestDeserializerStateDone,
    } tag;
} SmolRTSP_RequestDeserializerState;

/**
 * Deserializes @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 * @pre `state != NULL`
 */
SmolRTSP_DeserializeResult SmolRTSP_Request_deserialize(
    SmolRTSP_Request *restrict self, Slice99 *restrict data,
    SmolRTSP_RequestDeserializerState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs);

#endif // SMOLRTSP_REQUEST_H
