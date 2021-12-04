/**
 * @file
 * An RTSP response.
 */

#ifndef SMOLRTSP_RESPONSE_H
#define SMOLRTSP_RESPONSE_H

#include <smolrtsp/error.h>
#include <smolrtsp/header_map.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/response_line.h>

/**
 * The start state of #SmolRTSP_ResponseParseState.
 */
#define SMOLRTSP_RESPONSE_PARSE_STATE_INIT                                                         \
    (SmolRTSP_ResponseParseState) {                                                                \
        .start_line = SMOLRTSP_RESPONSE_LINE_PARSE_STATE_INIT,                                     \
        .tag = SmolRTSP_ResponseParseState_ResponseLine,                                           \
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
 * Serialises @p self into @p user_writer.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] user_writer The function to be provided with serialised data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Response_serialize(
    SmolRTSP_Response self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state of parsing of #SmolRTSP_Response.
 */
typedef struct {
    /**
     * The state of a response line being parsed.
     */
    SmolRTSP_ResponseLineParseState start_line;

    /**
     * What part of a response is being parsed right now.
     */
    enum {
        SmolRTSP_ResponseParseState_ResponseLine,
        SmolRTSP_ResponseParseState_HeaderMap,
        SmolRTSP_ResponseParseState_MessageBody,
        SmolRTSP_ResponseParseState_Done,
    } tag;
} SmolRTSP_ResponseParseState;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `state != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_Response_parse(
    SmolRTSP_Response *restrict self, CharSlice99 input,
    SmolRTSP_ResponseParseState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_Response_eq(SmolRTSP_Response lhs, SmolRTSP_Response rhs);

#endif // SMOLRTSP_RESPONSE_H
