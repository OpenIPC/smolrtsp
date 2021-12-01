#ifndef SMOLRTSP_SRC_COMMON_H
#define SMOLRTSP_SRC_COMMON_H

#include <slice99.h>

/**
 * Carriage-return + new-line represented as a data slice.
 */
#define SMOLRTSP_CRLF CharSlice99_from_str("\r\n")

#endif // SMOLRTSP_SRC_COMMON_H
