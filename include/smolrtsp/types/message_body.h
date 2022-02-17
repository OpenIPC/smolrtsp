/**
 * @file
 * An RTSP message body.
 */

#pragma once

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
    SmolRTSP_MessageBody *restrict self, CharSlice99 input, size_t content_length);

/**
 * Tests @p lhs and @p rhs for equality.
 */
bool SmolRTSP_MessageBody_eq(SmolRTSP_MessageBody lhs, SmolRTSP_MessageBody rhs);
