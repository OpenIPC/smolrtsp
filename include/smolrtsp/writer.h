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
 * @param ctx The user-supplied value.
 */
typedef void (*SmolRTSP_Writer)(CharSlice99 data, void *ctx);

/**
 * The same as #smolrtsp_write_slices but calculates an array length from variadic arguments (the
 * syntactically separated items of the array).
 */
#define SMOLRTSP_WRITE_SLICES(w, ctx, ...)                                                         \
    smolrtsp_write_slices(                                                                         \
        w, ctx, SLICE99_ARRAY_LEN((const CharSlice99[])__VA_ARGS__),                               \
        (const CharSlice99[])__VA_ARGS__)

/**
 * Sequentially writes all items in @p data to @p w.
 *
 * @param[in] w The function to be provided with data (possibly in chunks).
 * @param[in] ctx Some value provided to @p w on each write.
 * @param[in] len The number of items in @p data.
 * @param[in] data The data array which will be written to @p w, one-by-one.
 */
void smolrtsp_write_slices(
    SmolRTSP_Writer w, void *ctx, size_t len, const CharSlice99 data[restrict static len]);

/**
 * A writer that appends @p data to the array of characters @p buffer.
 *
 * @param[in] data The slice to the supplied data.
 * @param[out] buffer The character buffer to write to.
 *
 * @pre @p buffer shall be capable of holding `CharSlice99_size(data)` more characters, as required
 * by `strncat`.
 */
void smolrtsp_char_buffer_writer(CharSlice99 data, void *buffer);

/**
 * A user that which writes @p data to the file stream @p stream.
 *
 * @param[in] data The slice to the supplied data.
 * @param[out] stream The file stream of type `FILE *` to write to.
 */
void smolrtsp_file_stream_writer(CharSlice99 data, void *stream);

/** A writer that writes @p data to a file descriptor @p fd_ptr.
 *
 * @param[in] data The slice to the supplied data.
 * @param[out] fd_ptr The pointer to a file descriptor (`int`) to write to.
 */
void smolrtsp_fd_writer(CharSlice99 data, void *fd_ptr);

#endif // SMOLRTSP_USER_WRITER_H
