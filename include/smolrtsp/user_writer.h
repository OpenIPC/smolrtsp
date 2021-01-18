/**
 * @file
 * The writer interface.
 */

#ifndef SMOLRTSP_USER_WRITER_H
#define SMOLRTSP_USER_WRITER_H

#include <stddef.h>

#include <slice99.h>

/**
 * A user-supplied function accepting some data.
 *
 * @param[in] data The slice to the supplied data.
 * @param[in] user_cx The user-supplied value.
 */
typedef void (*SmolRTSP_UserWriter)(Slice99 data, void *user_cx);

#endif // SMOLRTSP_USER_WRITER_H
