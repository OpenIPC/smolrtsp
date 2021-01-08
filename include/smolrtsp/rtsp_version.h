/**
 * @file
 *  An RTSP version.
 */

#ifndef SMOLRTSP_RTSP_VERSION_H
#define SMOLRTSP_RTSP_VERSION_H

#include <smolrtsp/deserialization.h>
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
 * @pre `user_writer != NULL`
 */
void SmolRTSP_RTSPVersion_serialize(
    SmolRTSP_RTSPVersion self, SmolRTSP_UserWriter user_writer, void *user_cx);

SmolRTSP_DeserializeResult
SmolRTSP_RTSPVersion_deserialize(SmolRTSP_RTSPVersion *restrict self, Slice99 *restrict data);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 */
bool SmolRTSP_RTSPVersion_eq(SmolRTSP_RTSPVersion lhs, SmolRTSP_RTSPVersion rhs);

#endif // SMOLRTSP_RTSP_VERSION_H
