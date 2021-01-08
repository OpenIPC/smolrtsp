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
 * An RTSP request.
 */
typedef struct {
    SmolRTSP_RequestLine start_line;
    SmolRTSP_HeaderMap header_map;
    SmolRTSP_MessageBody body;
} SmolRTSP_Request;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Response_serialize(
    SmolRTSP_Request self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state in which #SmolRTSP_RequestDeserializer is located.
 */
typedef enum {
    SmolRTSP_RequestDeserializerStateRequestLine,
    SmolRTSP_RequestDeserializerStateHeaderMap,
    SmolRTSP_RequestDeserializerStateMessageBody,
    SmolRTSP_RequestDeserializerStateDone,
} SmolRTSP_RequestDeserializerState;

SmolRTSP_DeserializeResult SmolRTSP_Request_deserialize(
    SmolRTSP_Request *restrict self, Slice99 *restrict data,
    SmolRTSP_RequestDeserializerState *restrict state,
    SmolRTSP_RequestLineDeserializerState *restrict start_line_state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 */
bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs);

#endif // SMOLRTSP_REQUEST_H
