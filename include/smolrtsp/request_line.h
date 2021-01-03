/**
 * @file
 *  An RTSP request line.
 */

#ifndef SMOLRTSP_REQUEST_LINE_H
#define SMOLRTSP_REQUEST_LINE_H

#include <smolrtsp/crlf.h>
#include <smolrtsp/method.h>
#include <smolrtsp/request_uri.h>
#include <smolrtsp/rtsp_version.h>

#include <stdbool.h>

/**
 * An RTSP request line.
 */
typedef struct {
    SmolRTSP_Method method;
    SmolRTSP_RequestURI uri;
    SmolRTSP_RTSPVersion version;
} SmolRTSP_RequestLine;

/**
 * Serializes @p self to @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `self != NULL`
 * @pre `user_writer != NULL`
 */
void SmolRTSP_RequestLine_serialize(
    const SmolRTSP_RequestLine *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_RequestLine_eq(
    const SmolRTSP_RequestLine *restrict lhs, const SmolRTSP_RequestLine *restrict rhs);

#endif // SMOLRTSP_REQUEST_LINE_H
