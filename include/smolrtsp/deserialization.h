/**
 * @file
 * @brief Auxiliary declarations for deserialization.
 */

#ifndef SMOLRTSP_DESERIALIZATION_H
#define SMOLRTSP_DESERIALIZATION_H

#include <smolrtsp/slice.h>

#include <stddef.h>

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
    SmolRTSP_DeserializeResultNeedMore,
} SmolRTSP_DeserializeResult;

#endif // SMOLRTSP_DESERIALIZATION_H
