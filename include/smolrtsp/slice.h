/**
 * @file
 * @brief A slice of data.
 */

#ifndef SMOLRTSP_SLICE_H
#define SMOLRTSP_SLICE_H

#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stddef.h>

/**
 * An immutable slice of data.
 */
typedef struct {
    const void *data;

    /**
     * A size in bytes of \link #data \endlink.
     */
    size_t size;
} SmolRTSP_Slice;

SmolRTSP_Slice SmolRTSP_Slice_new(const void *data, size_t size);

/**
 * Returns a null immutable slice.
 *
 * @post \link #SmolRTSP_Slice.data \endlink is `NULL`.
 * @post \link #SmolRTSP_Slice.size \endlink is 0.
 */
SmolRTSP_Slice SmolRTSP_Slice_null(void);

/**
 * Decides whether @p self is a null slice or not.
 *
 * @return `true` if @p self is a null slice, otherwise `false`.
 *
 * @pre @p self shall not be `NULL`.
 */
bool SmolRTSP_Slice_is_null(SmolRTSP_Slice self);

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
 */
bool SmolRTSP_Slice_eq(const SmolRTSP_Slice *lhs, const SmolRTSP_Slice *rhs);

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 */
void SmolRTSP_Slice_serialize(
    const SmolRTSP_Slice *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A mutable slice of data.
 */
typedef struct {
    void *data;

    /**
     * A size in bytes of \link #data \endlink.
     */
    size_t size;
} SmolRTSP_MutSlice;

/**
 * Returns a null immutable slice.
 *
 * @post \link #SmolRTSP_Slice.data \endlink is `NULL`.
 * @post \link #SmolRTSP_Slice.size \endlink is 0.
 */
SmolRTSP_MutSlice SmolRTSP_MutSlice_null(void);

/**
 * Decides whether @p self is a null slice or not.
 *
 * @return `true` if @p self is a null slice, otherwise `false`.
 *
 * @pre @p self shall not be `NULL`.
 */
bool SmolRTSP_MutSlice_is_null(SmolRTSP_MutSlice self);

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
 */
bool SmolRTSP_MutSlice_eq(const SmolRTSP_MutSlice *lhs, const SmolRTSP_MutSlice *rhs);

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 */
void SmolRTSP_MutSlice_serialize(
    const SmolRTSP_MutSlice *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Casts @p self to an immutable slice.
 *
 * @pre @p self shall not be `NULL`.
 */
SmolRTSP_Slice SmolRTSP_MutSlice_to_const(SmolRTSP_MutSlice self);

#endif // SMOLRTSP_SLICE_H
