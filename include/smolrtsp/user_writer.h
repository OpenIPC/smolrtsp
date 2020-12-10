/**
 * @file
 * @brief A user-supplied writer interface.
 */

#ifndef SMOLRTSP_USER_WRITER_H
#define SMOLRTSP_USER_WRITER_H

#include <stddef.h>

/**
 * A user-supplied function accepting some data.
 *
 * @param[in] size A size in bytes of @p data.
 * @param[in] data A pointer to the actual data.
 * @param[in] user_cx A user-supplied value.
 */
typedef void (*SmolRTSP_UserWriter)(size_t size, const void *data, void *user_cx);

#endif // SMOLRTSP_USER_WRITER_H
