/**
 * @file
 * An RTSP response.
 */

#ifndef SMOLRTSP_RESPONSE_H
#define SMOLRTSP_RESPONSE_H

#include <smolrtsp/header_map.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/response_line.h>

/**
 * The start state of #SmolRTSP_ResponseDeserializerState.
 */
#define SMOLRTSP_RESPONSE_DESERIALIZER_STATE_INIT                                                  \
    (SmolRTSP_ResponseDeserializerState) {                                                         \
        .start_line = SMOLRTSP_RESPONSE_LINE_DESERIALIZER_STATE_INIT,                              \
        .tag = SmolRTSP_ResponseDeserializerState_ResponseLine,                                    \
    }

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
 * @param[in] self The instance to be serialized.
 * @param[in] user_writer The function to be provided with serialized data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Response_serialize(
    SmolRTSP_Response self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state of deserialization of #SmolRTSP_Response.
 */
typedef struct {
    /**
     * The state of a response line being deserialized.
     */
    SmolRTSP_ResponseLineDeserializerState start_line;

    /**
     * What part of a response is being deserialized right now.
     */
    enum {
        SmolRTSP_ResponseDeserializerState_ResponseLine,
        SmolRTSP_ResponseDeserializerState_HeaderMap,
        SmolRTSP_ResponseDeserializerState_MessageBody,
        SmolRTSP_ResponseDeserializerState_Done,
    } tag;
} SmolRTSP_ResponseDeserializerState;

/**
 * Deserializes @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 * @pre `state != NULL`
 */
SmolRTSP_DeserializeResult SmolRTSP_Response_deserialize(
    SmolRTSP_Response *restrict self, CharSlice99 *restrict data,
    SmolRTSP_ResponseDeserializerState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_Response_eq(SmolRTSP_Response lhs, SmolRTSP_Response rhs);

#endif // SMOLRTSP_RESPONSE_H
