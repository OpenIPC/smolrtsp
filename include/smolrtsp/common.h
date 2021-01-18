/**
 * @file
 * Common stuff.
 */

#ifndef SMOLRTSP_COMMON_H
#define SMOLRTSP_COMMON_H

#include <stddef.h>

#include <slice99.h>

/**
 * A result of each call to a deserializer.
 */
typedef enum {
    /**
     * All data have been parsed successfully.
     */
    SmolRTSP_DeserializeResultOk,

    /**
     * Something wrong happened.
     */
    SmolRTSP_DeserializeResultErr,

    /**
     * A deserializer need more data to continue.
     */
    SmolRTSP_DeserializeResultPending,
} SmolRTSP_DeserializeResult;

/**
 * Carriage-return + new-line represented as a data slice.
 */
#define SMOLRTSP_CRLF (Slice99_from_str("\r\n"))

#endif // SMOLRTSP_COMMON_H
