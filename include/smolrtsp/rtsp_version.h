/**
 * @file
 *  An RTSP version.
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
 * Serializes @p self to @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `self != NULL`
 * @pre `user_writer != NULL`
 */
void SmolRTSP_RTSPVersion_serialize(
    const SmolRTSP_RTSPVersion *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_RTSPVersion_eq(
    const SmolRTSP_RTSPVersion *restrict lhs, const SmolRTSP_RTSPVersion *restrict rhs);

#endif // SMOLRTSP_RTSP_VERSION_H
