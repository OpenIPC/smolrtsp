/**
 * @file
 * Common stuff.
 */

#ifndef SMOLRTSP_COMMON_H
#define SMOLRTSP_COMMON_H

/**
 * A result of each call to a parser.
 */
typedef enum {
    /**
     * All data have been parsed successfully.
     */
    SmolRTSP_ParseResult_Ok,

    /**
     * Something wrong happened.
     */
    SmolRTSP_ParseResult_Err,

    /**
     * A parser need more data to continue.
     */
    SmolRTSP_ParseResult_Pending,
} SmolRTSP_ParseResult;

#endif // SMOLRTSP_COMMON_H
