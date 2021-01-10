/**
 * @file
 *  An RTSP response.
 */

#ifndef SMOLRTSP_RESPONSE_H
#define SMOLRTSP_RESPONSE_H

#include <smolrtsp/header_map.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/response_line.h>

/**
 * An RTSP response.
 */
typedef struct {
    /**
     * The response line.
     */
    SmolRTSP_ResponseLine start_line;

    /**
     * The header map.
     */
    SmolRTSP_HeaderMap header_map;

    /**
     * The message body.
     */
    SmolRTSP_MessageBody body;
} SmolRTSP_Response;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Response_serialize(
    SmolRTSP_Response self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state of deserialization of #SmolRTSP_Response.
 */
typedef enum {
    SmolRTSP_ResponseDeserializerStateResponseLine,
    SmolRTSP_ResponseDeserializerStateHeaderMap,
    SmolRTSP_ResponseDeserializerStateMessageBody,
    SmolRTSP_ResponseDeserializerStateDone,
} SmolRTSP_ResponseDeserializerState;

SmolRTSP_DeserializeResult SmolRTSP_Response_deserialize(
    SmolRTSP_Response *restrict self, Slice99 *restrict data,
    SmolRTSP_ResponseDeserializerState *restrict state,
    SmolRTSP_ResponseLineDeserializerState *restrict start_line_state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 */
bool SmolRTSP_Response_eq(SmolRTSP_Response lhs, SmolRTSP_Response rhs);

#endif // SMOLRTSP_RESPONSE_H
