/**
 * @file
 *  Auxiliary declarations for deserialization.
 */

#ifndef SMOLRTSP_DESERIALIZATION_H
#define SMOLRTSP_DESERIALIZATION_H

#define SLICE99_INCLUDE_IO

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

#endif // SMOLRTSP_DESERIALIZATION_H
