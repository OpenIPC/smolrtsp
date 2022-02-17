/**
 * @file
 * An RTSP response.
 */

#pragma once

#include <smolrtsp/types/error.h>
#include <smolrtsp/types/header_map.h>
#include <smolrtsp/types/message_body.h>
#include <smolrtsp/types/response_line.h>

#include <stdbool.h>

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
 * @param[in] w The writer to be provided with serialised data.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_Response_serialize(SmolRTSP_Response self, SmolRTSP_Writer w);

/**
 * A state of parsing of #SmolRTSP_Response.
 */
typedef enum {
    SmolRTSP_ResponseParseState_ResponseLine,
    SmolRTSP_ResponseParseState_HeaderMap,
    SmolRTSP_ResponseParseState_MessageBody,
    SmolRTSP_ResponseParseState_Done,
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
 */
bool SmolRTSP_Response_eq(SmolRTSP_Response lhs, SmolRTSP_Response rhs);
