/**
 * @file
 * An RTSP response.
 */

#ifndef SMOLRTSP_RESPONSE_H
#define SMOLRTSP_RESPONSE_H

#include <smolrtsp/types/error.h>
#include <smolrtsp/types/header_map.h>
#include <smolrtsp/types/message_body.h>
#include <smolrtsp/types/response_line.h>

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
 * Serialises @p self into @p w.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The function to be provided with serialised data (possibly in chunks).
 * @param[in] w_ctx Some value provided to @p w on each write.
 *
 * @pre `w != NULL`
 */
void SmolRTSP_Response_serialize(SmolRTSP_Response self, SmolRTSP_Writer w, void *w_ctx);

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
