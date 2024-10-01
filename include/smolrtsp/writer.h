/**
 * @file
 * @brief The writer interface.
 */

#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <unistd.h>

#include <interface99.h>
#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The user-supplied data writer interface.
 */
#define SmolRTSP_Writer_IFACE                                                  \
                                                                               \
    /*                                                                         \
     * Writes @p data into itself.                                             \
     *                                                                         \
     * @param[in] data The slice of character data to write.                   \
     *                                                                         \
     * @return The number of bytes written or a negative value on error.       \
     */                                                                        \
    vfunc99(ssize_t, write, VSelf99, CharSlice99 data)                         \
                                                                               \
    /*                                                                         \
     * Lock writer to prevent race conditions on TCP interleaved channels      \
     */                                                                        \
    vfunc99(void, lock, VSelf99)                                               \
                                                                               \
    /*                                                                         \
     * Unlock writer locked by `lock`                                          \
     */                                                                        \
    vfunc99(void, unlock, VSelf99)                                             \
                                                                               \
    /*                                                                         \
     * Get current size of output buffer                                       \
     */                                                                        \
    vfunc99(size_t, filled, VSelf99)                                           \
                                                                               \
    /*                                                                         \
     * Writes a formatted string into itself.                                  \
     *                                                                         \
     * @param[in] fmt The `printf`-like format string.                         \
     *                                                                         \
     * @return The number of bytes written or a negative value on error.       \
     */                                                                        \
    vfunc99(int, writef, VSelf99, const char *restrict fmt, ...)               \
                                                                               \
    /*                                                                         \
     * The same as `printf` but accepts `va_list`.                             \
     */                                                                        \
    vfunc99(int, vwritef, VSelf99, const char *restrict fmt, va_list ap)

/**
 * Defines the `SmolRTSP_Writer` interface.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
interface99(SmolRTSP_Writer);

/**
 * The same as #smolrtsp_write_slices but calculates an array length from
 * variadic arguments (the syntactically separated items of the array).
 */
#define SMOLRTSP_WRITE_SLICES(w, ...)                                          \
    smolrtsp_write_slices(                                                     \
        w, SLICE99_ARRAY_LEN((const CharSlice99[])__VA_ARGS__),                \
        (const CharSlice99[])__VA_ARGS__)

/**
 * Sequentially writes all items in @p data to @p w.
 *
 * @param[in] w The writer to be provided with data.
 * @param[in] len The number of items in @p data.
 * @param[in] data The data array which will be written to @p w, one-by-one.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `w.self && w.vptr`
 */
ssize_t smolrtsp_write_slices(
    SmolRTSP_Writer w, size_t len,
    const CharSlice99 data[restrict static len]) SMOLRTSP_PRIV_MUST_USE;

/**
 * A writer that invokes `write` on a provided file descriptor.
 *
 * @pre `fd != NULL`
 */
SmolRTSP_Writer smolrtsp_fd_writer(int *fd) SMOLRTSP_PRIV_MUST_USE;

/**
 * A writer that invokes `fwrite` on a provided file pointer.
 *
 * @pre `stream != NULL`
 */
SmolRTSP_Writer smolrtsp_file_writer(FILE *stream) SMOLRTSP_PRIV_MUST_USE;

/**
 * A writer that invokes `strncat` on a provided buffer.
 *
 * @pre @p buffer shall be capable of holding all characters that will be
 * written into it.
 */
SmolRTSP_Writer smolrtsp_string_writer(char *buffer) SMOLRTSP_PRIV_MUST_USE;
