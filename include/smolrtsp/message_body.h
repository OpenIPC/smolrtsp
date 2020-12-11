/**
 * @file
 * @brief An RTSP message body.
 */

#ifndef SMOLRTSP_MESSAGE_BODY_H
#define SMOLRTSP_MESSAGE_BODY_H

#include <smolrtsp/slice.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stddef.h>

/**
 * An RTSP message body.
 */
typedef SmolRTSP_Slice SmolRTSP_MessageBody;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 */
void SmolRTSP_MessageBody_serialize(
    const SmolRTSP_MessageBody *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Compares @p lhs with @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre @p lhs shall not be `NULL`.
 * @pre @p rhs shall not be `NULL`.
 */
bool SmolRTSP_MessageBody_eq(const SmolRTSP_MessageBody *lhs, const SmolRTSP_MessageBody *rhs);

#endif // SMOLRTSP_MESSAGE_BODY_H
