/**
 * @file
 * An RTSP version.
 */

#ifndef SMOLRTSP_RTSP_VERSION_H
#define SMOLRTSP_RTSP_VERSION_H

#include <smolrtsp/common.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stdint.h>

/**
 * An RTSP version.
 */
typedef struct {
    /**
     * The major number.
     */
    uint_least8_t major;

    /**
     * The minor number.
     */
    uint_least8_t minor;
} SmolRTSP_RtspVersion;

/**
 * Serializes @p self to @p user_writer.
 *
 * @param[in] self The instance to be serialized.
 * @param[in] user_writer The function to be provided with serialized data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_RtspVersion_serialize(
    SmolRTSP_RtspVersion self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Deserializes @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 */
SmolRTSP_ParseResult
SmolRTSP_RtspVersion_parse(SmolRTSP_RtspVersion *restrict self, CharSlice99 *restrict data);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_RtspVersion_eq(SmolRTSP_RtspVersion lhs, SmolRTSP_RtspVersion rhs);

#endif // SMOLRTSP_RTSP_VERSION_H
