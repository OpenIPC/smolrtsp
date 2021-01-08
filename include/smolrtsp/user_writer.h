/**
 * @file
 * The writer interface.
 */

#ifndef SMOLRTSP_USER_WRITER_H
#define SMOLRTSP_USER_WRITER_H

#include <stddef.h>

/**
 * A user-supplied function accepting some data.
 *
 * @param[in] size The size in bytes of the actual data.
 * @param[in] data The pointer to the actual data.
 * @param[in] user_cx The user-supplied value.
 */
typedef void (*SmolRTSP_UserWriter)(size_t size, const void *data, void *user_cx);

#endif // SMOLRTSP_USER_WRITER_H
