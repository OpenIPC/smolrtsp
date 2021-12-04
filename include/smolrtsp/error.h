/**
 * @file
 * Common stuff.
 */

#ifndef SMOLRTSP_COMMON_H
#define SMOLRTSP_COMMON_H

#include <stdbool.h>
#include <stddef.h>

#include <datatype99.h>
#include <slice99.h>

/**
 * Possible variants of #SmolRTSP_TypeMismatchError.
 */
typedef enum {
    /**
     * Expected an integer.
     */
    SmolRTSP_TypeMismatchErrorKind_Int,

    /**
     * Expected an identifier.
     */
    SmolRTSP_TypeMismatchErrorKind_Ident,

    /**
     * Expected a header name.
     */
    SmolRTSP_TypeMismatchErrorKind_HeaderName,
} SmolRTSP_TypeMismatchErrorKind;

// clang-format off
datatype99(
    SmolRTSP_ParseError,

    /**
     * An invalid value of the `Content-Length` header was specified.
     */
    (SmolRTSP_ParseError_ContentLength,
        CharSlice99 /* the value of this header */),

    /**
     * Two given strings are ineqal.
     */
    (SmolRTSP_ParseError_StrMismatch,
        CharSlice99 /* expected */,
        CharSlice99 /* actual */),

    /**
     * Failed to parse an integer.
     */
    (SmolRTSP_ParseError_TypeMismatch,
        SmolRTSP_TypeMismatchErrorKind,
        CharSlice99 /* the erroneous string */),

    /**
     * An attempt to add a header to a full header map.
     */
    (SmolRTSP_ParseError_HeaderMapOverflow)
);
// clang-format on

typedef struct {
    /**
     * A byte offset from the beginning of input.
     */
    size_t offset;

    /**
     * Whether parsing has completed or needs more data (partial).
     */
    bool is_complete;
} SmolRTSP_ParseStatus;

/**
 * Creates a **partial** parse status with the byte offset @p offset (from the beginning of input).
 */
SmolRTSP_ParseStatus SmolRTSP_ParseStatus_partial(size_t offset);

/**
 * The same as #SmolRTSP_ParseStatus_partial but for a complete status.
 */
SmolRTSP_ParseStatus SmolRTSP_ParseStatus_complete(size_t offset);

// clang-format off
datatype99(
    SmolRTSP_ParseResult,
    (SmolRTSP_ParseResult_Success, SmolRTSP_ParseStatus),
    (SmolRTSP_ParseResult_Failure, SmolRTSP_ParseError)
);
// clang-format on

/**
 * Creates a **successful** and **partial** parse result with the byte offset @p offset (from the
 * beginning of input).
 */
SmolRTSP_ParseResult SmolRTSP_ParseResult_partial(size_t offset);

/**
 * The same as #SmolRTSP_ParseResult_partial but for a complete result.
 */
SmolRTSP_ParseResult SmolRTSP_ParseResult_complete(size_t offset);

/**
 * Returns whether @p self is both **successful** and **partial**.
 */
bool SmolRTSP_ParseResult_is_partial(SmolRTSP_ParseResult self);

/**
 * The same as #SmolRTSP_ParseResult_is_partial but for a complete result.
 */
bool SmolRTSP_ParseResult_is_complete(SmolRTSP_ParseResult self);

#endif // SMOLRTSP_COMMON_H
