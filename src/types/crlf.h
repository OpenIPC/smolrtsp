#pragma once

#include <slice99.h>

/**
 * Carriage-return + new-line represented as a data slice.
 */
#define SMOLRTSP_CRLF CharSlice99_from_str("\r\n")

#define CHK_WRITE_ERR(result, ret)                                                                 \
    do {                                                                                           \
        if (ret < 0) {                                                                             \
            return result;                                                                         \
        } else {                                                                                   \
            result += ret;                                                                         \
        }                                                                                          \
    } while (0)
