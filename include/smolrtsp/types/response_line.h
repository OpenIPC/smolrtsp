/**
 * @file
 * An RTSP response line.
 */

#pragma once

#include <smolrtsp/types/error.h>
#include <smolrtsp/types/reason_phrase.h>
#include <smolrtsp/types/rtsp_version.h>
#include <smolrtsp/types/status_code.h>

/**
 * The start state of #SmolRTSP_ResponseLineParseState.
 */
#define SMOLRTSP_RESPONSE_LINE_PARSE_STATE_INIT                                                    \
    (SmolRTSP_ResponseLineParseState) {                                                            \
        .tag = SmolRTSP_ResponseLineParseState_RtspVersion,                                        \
    }

/**
 * An RTSP response line.
 */
typedef struct {
    /**
     * The RTSP version used.
     */
    SmolRTSP_RtspVersion version;

    /**
     * The status code.
     */
    SmolRTSP_StatusCode code;

    /**
     * The reason phrase.
     */
    SmolRTSP_ReasonPhrase reason;
} SmolRTSP_ResponseLine;

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
ssize_t SmolRTSP_ResponseLine_serialize(SmolRTSP_ResponseLine self, SmolRTSP_Writer w);

/**
 * A state of parsing of #SmolRTSP_ResponseLine.
 */
typedef struct {
    /**
     * What part of a respnose line is being parsed right now.
     */
    enum {
        SmolRTSP_ResponseLineParseState_RtspVersion,
        SmolRTSP_ResponseLineParseState_StatusCode,
        SmolRTSP_ResponseLineParseState_ReasonPhrase,
        SmolRTSP_ResponseLineParseState_Done,
    } tag;
} SmolRTSP_ResponseLineParseState;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `state != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_ResponseLine_parse(
    SmolRTSP_ResponseLine *restrict self, CharSlice99 input,
    SmolRTSP_ResponseLineParseState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_ResponseLine_eq(SmolRTSP_ResponseLine lhs, SmolRTSP_ResponseLine rhs);
