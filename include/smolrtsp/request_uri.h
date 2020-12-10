/**
 * @file
 * @brief An RTSP request URI.
 */

#ifndef SMOLRTSP_REQUEST_URI_H
#define SMOLRTSP_REQUEST_URI_H

#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>

/**
 * An RTSP request URI.
 */
typedef struct {
    char data[SMOLRTSP_REQUEST_URI_SIZE];
} SmolRTSP_RequestURI;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 */
void SmolRTSP_RequestURI_serialize(
    const SmolRTSP_RequestURI *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Compares @p lhs with @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre @p lhs shall not be `NULL`.
 * @pre @p rhs shall not be `NULL`.
 */
bool SmolRTSP_RequestURI_eq(const SmolRTSP_RequestURI *lhs, const SmolRTSP_RequestURI *rhs);

#endif // SMOLRTSP_REQUEST_URI_H
