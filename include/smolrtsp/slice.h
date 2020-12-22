/**
 * @file
 *  A slice of data.
 *
 * A slice is null if its pointer to data is `NULL`. A slice is empty is its size compares equal to
 * zero.
 */

#ifndef SMOLRTSP_SLICE_H
#define SMOLRTSP_SLICE_H

#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/**
 * An immutable slice of data.
 */
typedef struct {
    const void *ptr;

    /**
     * A size in bytes of \link #data \endlink.
     */
    size_t size;
} SmolRTSP_Slice;

/**
 * Constructs a slice.
 *
 * @pre The first @p size bytes of @p data must be available for reading.
 */
SmolRTSP_Slice SmolRTSP_Slice_new(const void *data, size_t size);

/**
 * Returns a null and empty slice.
 */
SmolRTSP_Slice SmolRTSP_Slice_null(void);

/**
 * Checks whether @p self is a null slice or not.
 *
 * @return `true` if @p self is a null slice, otherwise `false`.
 */
bool SmolRTSP_Slice_is_null(SmolRTSP_Slice self);

/**
 * Checks whether @p self is an empty slice or not.
 *
 * @return `true` if @p self is an empty slice, otherwise `false`.
 */
bool SmolRTSP_Slice_is_empty(SmolRTSP_Slice self);

/**
 * Advances @p self by @p offset bytes.
 *
 * @pre @p self is not a null slice.
 * @pre `offset <= self.size`.
 * @pre A memory area [`self.size + offset`; `self.size - offset`) must be available for reading.
 */
SmolRTSP_Slice SmolRTSP_Slice_advance(SmolRTSP_Slice self, size_t offset);

/**
 * Moves @p self by @p offset positions to the left.
 *
 * @pre @p self is not a null slice.
 * @pre `offset <= self.size`.
 * @pre A memory area [`self.size - offset`; `self.size + offset`) must be available for reading.
 */
SmolRTSP_Slice SmolRTSP_Slice_go_back(SmolRTSP_Slice self, size_t offset);

/**
 * Constructs a slice from @p str.
 *
 * @note If @p str is `NULL`, then the returned slice is also null.
 */
SmolRTSP_Slice SmolRTSP_Slice_from_str(const char *str);

/**
 * Compares @p lhs with @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre @p lhs shall not be `NULL`.
 * @pre @p rhs shall not be `NULL`.
 *
 * @note Null slices compare equal with each other.
 */
bool SmolRTSP_Slice_eq(const SmolRTSP_Slice *restrict lhs, const SmolRTSP_Slice *restrict rhs);

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 * @pre @p self shall not be a null slice.
 */
void SmolRTSP_Slice_serialize(
    const SmolRTSP_Slice *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Prints @p data to @p stream.
 *
 * @pre @p self shall not be a null slice.
 */
void SmolRTSP_Slice_print_s_to_file(SmolRTSP_Slice self, FILE *stream);

/**
 * Prints @p data to `stdout`.
 *
 * @pre @p self shall not be a null slice.
 */
void SmolRTSP_Slice_print_s(SmolRTSP_Slice self);

/**
 * The same as #SmolRTSP_Slice_print_s_to_file but places `\n` afterwards.
 *
 * @pre @p self shall not be a null slice.
 */
void SmolRTSP_Slice_print_s_to_file_ln(SmolRTSP_Slice self, FILE *stream);

/**
 * The same as #SmolRTSP_Slice_print_s but places `\n` afterwards.
 *
 * @pre @p self shall not be a null slice.
 */
void SmolRTSP_Slice_print_s_ln(SmolRTSP_Slice self);

/**
 * A mutable slice of data.
 */
typedef struct {
    void *ptr;

    /**
     * A size in bytes of \link #data \endlink.
     */
    size_t size;
} SmolRTSP_MutSlice;

/**
 * Constructs a slice.
 *
 * @pre The first @p size bytes of @p data must be available for reading and writing.
 */
SmolRTSP_MutSlice SmolRTSP_MutSlice_new(void *data, size_t size);

/**
 * Returns a null and empty slice.
 */
SmolRTSP_MutSlice SmolRTSP_MutSlice_null(void);

/**
 * Decides whether @p self is a null slice or not.
 *
 * @return `true` if @p self is a null slice, otherwise `false`.
 */
bool SmolRTSP_MutSlice_is_null(SmolRTSP_MutSlice self);

/**
 * Checks whether @p self is an empty slice or not.
 *
 * @return `true` if @p self is an empty slice, otherwise `false`.
 */
bool SmolRTSP_MutSlice_is_empty(SmolRTSP_MutSlice self);

/**
 * Advances @p self by @p offset bytes.
 *
 * @pre @p self is not a null slice.
 * @pre `offset <= self.size`.
 */
SmolRTSP_MutSlice SmolRTSP_MutSlice_advance(SmolRTSP_MutSlice self, size_t offset);

/**
 * Constructs a slice from @p str.
 *
 * @note If @p str is `NULL`, then the returned slice is also null.
 */
SmolRTSP_MutSlice SmolRTSP_MutSlice_from_str(char *str);

/**
 * Compares @p lhs with @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre @p lhs shall not be `NULL`.
 * @pre @p rhs shall not be `NULL`.
 *
 * @note Null slices compare equal with each other.
 */
bool SmolRTSP_MutSlice_eq(
    const SmolRTSP_MutSlice *restrict lhs, const SmolRTSP_MutSlice *restrict rhs);

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 * @pre @p self shall not be a null slice.
 */
void SmolRTSP_MutSlice_serialize(
    const SmolRTSP_MutSlice *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Casts @p self to an immutable slice.
 */
SmolRTSP_Slice SmolRTSP_MutSlice_to_const(SmolRTSP_MutSlice self);

#endif // SMOLRTSP_SLICE_H
