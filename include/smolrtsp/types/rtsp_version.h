/**
 * @file
 * @brief An RTSP version.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/error.h>
#include <smolrtsp/writer.h>

#include <stdbool.h>
#include <stdint.h>

#include <slice99.h>

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
 * @param[in] w The writer to be provided with serialised data.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `self != NULL`
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_RtspVersion_serialize(
    const SmolRTSP_RtspVersion *restrict self,
    SmolRTSP_Writer w) SMOLRTSP_PRIV_MUST_USE;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_RtspVersion_parse(
    SmolRTSP_RtspVersion *restrict self,
    CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_RtspVersion_eq(
    const SmolRTSP_RtspVersion *restrict lhs,
    const SmolRTSP_RtspVersion *restrict rhs) SMOLRTSP_PRIV_MUST_USE;
