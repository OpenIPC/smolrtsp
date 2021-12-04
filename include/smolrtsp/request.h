/**
 * @file
 * An RTSP request.
 */

#ifndef SMOLRTSP_REQUEST_H
#define SMOLRTSP_REQUEST_H

#include <smolrtsp/error.h>
#include <smolrtsp/header_map.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/request_line.h>

#include <stdbool.h>

/**
 * The start state of #SmolRTSP_RequestParseState.
 */
#define SMOLRTSP_REQUEST_PARSE_STATE_INIT                                                          \
    (SmolRTSP_RequestParseState) {                                                                 \
        .start_line = SMOLRTSP_REQUEST_LINE_PARSE_STATE_INIT,                                      \
        .tag = SmolRTSP_RequestParseState_RequestLine,                                             \
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
 * Serialises @p self into @p user_writer.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] user_writer The function to be provided with serialised data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Request_serialize(
    SmolRTSP_Request self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state of parsing of #SmolRTSP_Request.
 */
typedef struct {
    /**
     * The state of a request line being parsed.
     */
    SmolRTSP_RequestLineParseState start_line;

    /**
     * What part of a request is being parsed right now.
     */
    enum {
        SmolRTSP_RequestParseState_RequestLine,
        SmolRTSP_RequestParseState_HeaderMap,
        SmolRTSP_RequestParseState_MessageBody,
        SmolRTSP_RequestParseState_Done,
    } tag;
} SmolRTSP_RequestParseState;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `state != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_Request_parse(
    SmolRTSP_Request *restrict self, CharSlice99 input, SmolRTSP_RequestParseState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs);

#endif // SMOLRTSP_REQUEST_H
