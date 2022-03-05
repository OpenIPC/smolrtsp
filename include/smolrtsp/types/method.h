/**
 * @file
 * @brief An RTSP method.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/error.h>

#include <stdbool.h>

#include <slice99.h>

/**
 * An RTSP method.
 */
typedef CharSlice99 SmolRTSP_Method;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_Method_parse(
    SmolRTSP_Method *restrict self, CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_Method_eq(
    const SmolRTSP_Method *restrict lhs,
    const SmolRTSP_Method *restrict rhs) SMOLRTSP_PRIV_MUST_USE;

/**
 * `OPTIONS`.
 */
#define SMOLRTSP_METHOD_OPTIONS (CharSlice99_from_str("OPTIONS"))

/**
 * `DESCRIBE`.
 */
#define SMOLRTSP_METHOD_DESCRIBE (CharSlice99_from_str("DESCRIBE"))

/**
 * `ANNOUNCE`.
 */
#define SMOLRTSP_METHOD_ANNOUNCE (CharSlice99_from_str("ANNOUNCE"))

/**
 * `SETUP`.
 */
#define SMOLRTSP_METHOD_SETUP (CharSlice99_from_str("SETUP"))

/**
 * `PLAY`.
 */
#define SMOLRTSP_METHOD_PLAY (CharSlice99_from_str("PLAY"))

/**
 * `PAUSE`.
 */
#define SMOLRTSP_METHOD_PAUSE (CharSlice99_from_str("PAUSE"))

/**
 * `TEARDOWN`.
 */
#define SMOLRTSP_METHOD_TEARDOWN (CharSlice99_from_str("TEARDOWN"))

/**
 * `GET_PARAMETER`.
 */
#define SMOLRTSP_METHOD_GET_PARAMETER (CharSlice99_from_str("GET_PARAMETER"))

/**
 * `SET_PARAMETER`.
 */
#define SMOLRTSP_METHOD_SET_PARAMETER (CharSlice99_from_str("SET_PARAMETER"))

/**
 * `REDIRECT`.
 */
#define SMOLRTSP_METHOD_REDIRECT (CharSlice99_from_str("REDIRECT"))

/**
 * `RECORD`.
 */
#define SMOLRTSP_METHOD_RECORD (CharSlice99_from_str("RECORD"))
