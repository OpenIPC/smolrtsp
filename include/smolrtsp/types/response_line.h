/**
 * @file
 * An RTSP response line.
 */

#pragma once

#include <smolrtsp/types/error.h>
#include <smolrtsp/types/reason_phrase.h>
#include <smolrtsp/types/rtsp_version.h>
#include <smolrtsp/types/status_code.h>

#include <stdbool.h>

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
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult
SmolRTSP_ResponseLine_parse(SmolRTSP_ResponseLine *restrict self, CharSlice99 input);

/**
 * Tests @p lhs and @p rhs for equality.
 */
bool SmolRTSP_ResponseLine_eq(SmolRTSP_ResponseLine lhs, SmolRTSP_ResponseLine rhs);
