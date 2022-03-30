/**
 * @file
 * @brief An RTSP message body.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/error.h>

#include <stdbool.h>
#include <stddef.h>

#include <slice99.h>

/**
 * An RTSP message body.
 */
typedef CharSlice99 SmolRTSP_MessageBody;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_MessageBody_parse(
    SmolRTSP_MessageBody *restrict self, CharSlice99 input,
    size_t content_length) SMOLRTSP_PRIV_MUST_USE;

/**
 * Returns an empty message body.
 */
SmolRTSP_MessageBody SmolRTSP_MessageBody_empty(void) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_MessageBody_eq(
    const SmolRTSP_MessageBody *restrict lhs,
    const SmolRTSP_MessageBody *restrict rhs) SMOLRTSP_PRIV_MUST_USE;
