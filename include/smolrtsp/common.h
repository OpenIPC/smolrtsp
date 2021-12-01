/**
 * @file
 * Common stuff.
 */

#ifndef SMOLRTSP_COMMON_H
#define SMOLRTSP_COMMON_H

/**
 * A result of each call to a deserializer.
 */
typedef enum {
    /**
     * All data have been parsed successfully.
     */
    SmolRTSP_DeserializeResult_Ok,

    /**
     * Something wrong happened.
     */
    SmolRTSP_DeserializeResult_Err,

    /**
     * A deserializer need more data to continue.
     */
    SmolRTSP_DeserializeResult_Pending,
} SmolRTSP_DeserializeResult;

#endif // SMOLRTSP_COMMON_H
