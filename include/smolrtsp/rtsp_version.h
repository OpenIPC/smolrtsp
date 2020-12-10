/**
 * @file
 * @brief An RTSP version.
 */

#ifndef SMOLRTSP_RTSP_VERSION_H
#define SMOLRTSP_RTSP_VERSION_H

#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stdint.h>

/**
 * An RTSP version.
 */
typedef struct {
    uint_least8_t major;
    uint_least8_t minor;
} SmolRTSP_RTSPVersion;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 */
void SmolRTSP_RTSPVersion_serialize(
    const SmolRTSP_RTSPVersion *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Compares @p lhs with @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre @p lhs shall not be `NULL`.
 * @pre @p rhs shall not be `NULL`.
 */
bool SmolRTSP_RTSPVersion_eq(const SmolRTSP_RTSPVersion *lhs, const SmolRTSP_RTSPVersion *rhs);

#endif // SMOLRTSP_RTSP_VERSION_H
