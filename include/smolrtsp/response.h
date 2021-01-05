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
    SmolRTSP_ResponseLine start_line;
    SmolRTSP_HeaderMap header_map;
    SmolRTSP_MessageBody body;
} SmolRTSP_Response;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `self != NULL`
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Response_serialize(
    const SmolRTSP_Response *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * `lhs != NULL`
 * `rhs != NULL`
 */
bool SmolRTSP_Response_eq(
    const SmolRTSP_Response *restrict lhs, const SmolRTSP_Response *restrict rhs);

#endif // SMOLRTSP_RESPONSE_H
