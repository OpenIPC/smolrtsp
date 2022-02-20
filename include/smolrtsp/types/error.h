/**
 * @file
 * Possible parsing errors.
 */

#pragma once

#include <smolrtsp/writer.h>

#include <stdbool.h>
#include <stddef.h>

#include <datatype99.h>
#include <slice99.h>

/**
 * Types of data that can be failed to parse.
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

/**
 * An error that might occur during parsing.
 *
 * ## Variants
 *
 *  - `ContentLength` -- An invalid value of the `Content-Length` header was
 * specified. Arguments:
 *    1. The value of this header.
 *  - `StrMismatch` -- Two given strings are uneqal. Arguments:
 *    1. Expected string.
 *    2. Actual string.
 *  - `TypeMismatch` -- Failed to parse an item. Arguments:
 *    1. A type of item failed to parse.
 *    2. The erroneous string.
 *  - `HeaderMapOverflow` -- An attempt to add a header to a full header map.
 *
 * See [Datatype99](https://github.com/Hirrolot/datatype99) for the macro usage.
 */

// clang-format off
datatype99(
    SmolRTSP_ParseError,
    (SmolRTSP_ParseError_ContentLength, CharSlice99),
    (SmolRTSP_ParseError_StrMismatch, CharSlice99, CharSlice99),
    (SmolRTSP_ParseError_TypeMismatch, SmolRTSP_ParseType, CharSlice99),
    (SmolRTSP_ParseError_HeaderMapOverflow)
);
// clang-format on

/**
 * Prints @p self into @p w.
 *
 * @param[in] self The error to print.
 * @param[in] w The writer to be provided with data.
 */
void SmolRTSP_ParseError_print(SmolRTSP_ParseError self, SmolRTSP_Writer w);

typedef struct {
    /**
     * A byte offset from the beginning of input. Only meaningful if
     * #SmolRTSP_ParseStatus.is_complete is true, otherwise it is set to zero.
     */
    size_t offset;

    /**
     * Whether parsing has completed or needs more data (partial).
     */
    bool is_complete;
} SmolRTSP_ParseStatus;

/**
 * Creates a **partial** parse status.
 */
SmolRTSP_ParseStatus SmolRTSP_ParseStatus_partial(void);

/**
 * Creates a **complete** parse status with the byte offset @p offset (from the beginning of input).
 */
SmolRTSP_ParseStatus SmolRTSP_ParseStatus_complete(size_t offset);

/**
 * A result of parsing (either success or failure).
 *
 * See [Datatype99](https://github.com/Hirrolot/datatype99) for the macro usage.
 */

// clang-format off
datatype99(
    SmolRTSP_ParseResult,
    (SmolRTSP_ParseResult_Success, SmolRTSP_ParseStatus),
    (SmolRTSP_ParseResult_Failure, SmolRTSP_ParseError)
);
// clang-format on

/**
 * Creates a **successful** and **partial** parse result.
 */
SmolRTSP_ParseResult SmolRTSP_ParseResult_partial(void);

/**
 * Creates a **successful** and **complete** parse result with the byte offset @p offset (from the
 * beginning of input).
 */
SmolRTSP_ParseResult SmolRTSP_ParseResult_complete(size_t offset);

/**
 * Returns whether @p self is successful.
 */
bool SmolRTSP_ParseResult_is_success(SmolRTSP_ParseResult self);

/**
 * Returns whether @p self is a failure.
 */
bool SmolRTSP_ParseResult_is_failure(SmolRTSP_ParseResult self);

/**
 * Returns whether @p self is both **successful** and **partial**.
 */
bool SmolRTSP_ParseResult_is_partial(SmolRTSP_ParseResult self);

/**
 * The same as #SmolRTSP_ParseResult_is_partial but for a complete result.
 */
bool SmolRTSP_ParseResult_is_complete(SmolRTSP_ParseResult self);
