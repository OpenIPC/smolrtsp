#ifndef SMOLRTSP_RESPONSE_LINE_H
#define SMOLRTSP_RESPONSE_LINE_H

#include <smolrtsp/reason_phrase.h>
#include <smolrtsp/rtsp_version.h>
#include <smolrtsp/status_code.h>

/**
 * An RTSP response line.
 */
typedef struct {
    SmolRTSP_RTSPVersion version;
    SmolRTSP_StatusCode code;
    SmolRTSP_ReasonPhrase reason;
} SmolRTSP_ResponseLine;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `self != NULL`
 * @pre `user_writer != NULL`
 */
void SmolRTSP_ResponseLine_serialize(
    const SmolRTSP_ResponseLine *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_ResponseLine_eq(
    const SmolRTSP_ResponseLine *restrict lhs, const SmolRTSP_ResponseLine *restrict rhs);

#endif // SMOLRTSP_RESPONSE_LINE_H
