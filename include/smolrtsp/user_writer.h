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
 * @param user_cx The user-supplied value.
 */
typedef void (*SmolRTSP_UserWriter)(Slice99 data, void *user_cx);

/**
 * A writer which appends @p data to the array of characters @p buffer.
 *
 * @param[in] data The slice to the supplied data.
 * @param[out] buffer The character buffer to write to.
 *
 * @pre @p buffer shall be capable of holding `Slice99_size(data)` more characters, as required by
 * `strncat`.
 */
void smolrtsp_char_buffer_writer(Slice99 data, void *buffer);

/**
 * A user writer which writes @p data to the file stream @p stream.
 *
 * @param[in] data The slice to the supplied data.
 * @param[out] stream The file stream of type `FILE *` to write to.
 */
void smolrtsp_file_stream_writer(Slice99 data, void *stream);

#endif // SMOLRTSP_USER_WRITER_H
