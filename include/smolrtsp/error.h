/**
 * @file
 * Common stuff.
 */

#ifndef SMOLRTSP_COMMON_H
#define SMOLRTSP_COMMON_H

#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stddef.h>

#include <datatype99.h>
#include <slice99.h>

/**
 * Possible variants of #SmolRTSP_TypeMismatchError.
 */
typedef enum {
    /**
     * An integer (`-34`, `0`, `123`).
     */
    SmolRTSP_ParseType_Int,

    /**
     * An identifier (`abc`).
     */
    SmolRTSP_ParseType_Ident,

    /**
     * A header name (`Content-Length`, `Authorization`).
     */
    SmolRTSP_ParseType_HeaderName,
} SmolRTSP_ParseType;

/**
 * Returns a string representation of @p self.
 */
const char *SmolRTSP_ParseType_str(SmolRTSP_ParseType self);

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
        SmolRTSP_ParseType,
        CharSlice99 /* the erroneous string */),

    /**
     * An attempt to add a header to a full header map.
     */
    (SmolRTSP_ParseError_HeaderMapOverflow)
);
// clang-format on

/**
 * Prints @p self to @p w.
 *
 * @param[in] self The error to print.
 * @param[in] w The function to be provided with data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p w on each write.
 */
void SmolRTSP_ParseError_print(SmolRTSP_ParseError self, SmolRTSP_UserWriter w, void *user_cx);

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
