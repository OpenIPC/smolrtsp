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
 * The largest `Content-Length` a parsed request or response may declare.
 *
 * Larger values are rejected with #SmolRTSP_ParseError_ContentLength rather
 * than waited for: a peer that declares a body of `SIZE_MAX` bytes would
 * otherwise keep a request Partial, and its bytes buffered, for as long as it
 * keeps sending. RTSP bodies are SDP and parameter lists, a few kilobytes at
 * most. The parsers are compiled with this value, so an override has to be in
 * the library's own build: configure with `-DSMOLRTSP_MAX_CONTENT_LENGTH=<n>`,
 * which also defines it for consumers of the target. Values above `SIZE_MAX`
 * are treated as `SIZE_MAX`.
 */
#ifndef SMOLRTSP_MAX_CONTENT_LENGTH
#define SMOLRTSP_MAX_CONTENT_LENGTH 65536
#endif

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
