/**
 * @file
 * @brief An RTSP request URI.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/error.h>

#include <stdbool.h>

#include <slice99.h>

/**
 * An RTSP request URI.
 */
typedef CharSlice99 SmolRTSP_RequestUri;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_RequestUri_parse(
    SmolRTSP_RequestUri *restrict self,
    CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_RequestUri_eq(
    const SmolRTSP_RequestUri *restrict lhs,
    const SmolRTSP_RequestUri *restrict rhs) SMOLRTSP_PRIV_MUST_USE;
