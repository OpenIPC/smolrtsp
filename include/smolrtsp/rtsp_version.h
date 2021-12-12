/**
 * @file
 * An RTSP version.
 */

#ifndef SMOLRTSP_RTSP_VERSION_H
#define SMOLRTSP_RTSP_VERSION_H

#include <smolrtsp/error.h>
#include <smolrtsp/writer.h>

#include <stdbool.h>
#include <stdint.h>

/**
 * An RTSP version.
 */
typedef struct {
    /**
     * The major number.
     */
    uint8_t major;

    /**
     * The minor number.
     */
    uint8_t minor;
} SmolRTSP_RtspVersion;

/**
 * Serialises @p self into @p w.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The function to be provided with serialised data (possibly in chunks).
 * @param[in] w_ctx Some value provided to @p w on each write.
 *
 * @pre `w != NULL`
 */
void SmolRTSP_RtspVersion_serialize(SmolRTSP_RtspVersion self, SmolRTSP_Writer w, void *w_ctx);

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult
SmolRTSP_RtspVersion_parse(SmolRTSP_RtspVersion *restrict self, CharSlice99 input);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_RtspVersion_eq(SmolRTSP_RtspVersion lhs, SmolRTSP_RtspVersion rhs);

#endif // SMOLRTSP_RTSP_VERSION_H
