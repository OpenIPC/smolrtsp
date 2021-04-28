/**
 * @file
 * An RTSP header map.
 */

#ifndef SMOLRTSP_HEADER_MAP_H
#define SMOLRTSP_HEADER_MAP_H

#include <smolrtsp/common.h>
#include <smolrtsp/header.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <slice99.h>

/**
 * Creates new #SmolRTSP_HeaderMap from an array expression of #SmolRTSP_Header.
 */
#define SmolRTSP_HeaderMap_from_array(...)                                                         \
    ((SmolRTSP_HeaderMap){                                                                         \
        .headers = (__VA_ARGS__),                                                                  \
        .len = SLICE99_ARRAY_LEN(__VA_ARGS__),                                                     \
        .capacity = SLICE99_ARRAY_LEN(__VA_ARGS__),                                                \
    })

/**
 * Creates new #SmolRTSP_HeaderMap with a specified capacity and the length of zero.
 */
#define SmolRTSP_HeaderMap_with_capacity(capacity_)                                                \
    ((SmolRTSP_HeaderMap){                                                                         \
        .headers = (SmolRTSP_Header[capacity_]){0},                                                \
        .len = 0,                                                                                  \
        .capacity = capacity_,                                                                     \
    })

/**
 * An RTSP header map.
 */
typedef struct {
    /**
     * The pointer to an array of headers;
     */
    SmolRTSP_Header *headers;

    /**
     * The count of elements currently in #headers.
     */
    size_t len;

    /**
     * The count of elements #headers is able to contain.
     */
    size_t capacity;
} SmolRTSP_HeaderMap;

/**
 * Returns the empty header map.
 */
SmolRTSP_HeaderMap SmolRTSP_HeaderMap_empty(void);

/**
 * Returns a value associated with @p key within @p self.
 */
Slice99Maybe SmolRTSP_HeaderMap_find(SmolRTSP_HeaderMap self, Slice99 key);

/**
 * Serializes @p self to @p user_writer.
 *
 * @param[in] self The instance to be serialized.
 * @param[in] user_writer The function to be provided with serialized data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_HeaderMap_serialize(
    SmolRTSP_HeaderMap self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Deserializes @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 */
SmolRTSP_DeserializeResult
SmolRTSP_HeaderMap_deserialize(SmolRTSP_HeaderMap *restrict self, Slice99 *restrict data);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_HeaderMap_eq(SmolRTSP_HeaderMap lhs, SmolRTSP_HeaderMap rhs);

/**
 * Tests whether @p self is full (no more space left for an additional header) or not.
 *
 * @return `true` if @p self is full, `false` otherwise
 */
bool SmolRTSP_HeaderMap_is_full(SmolRTSP_HeaderMap self);

/**
 * Pretty-prints @p self to @p stream.
 *
 * @pre `stream != NULL`
 */
void SmolRTSP_HeaderMap_dbg_to_file(SmolRTSP_HeaderMap self, FILE *stream);

/**
 * Pretty-prints @p self to `stdout`.
 */
void SmolRTSP_HeaderMap_dbg(SmolRTSP_HeaderMap self);

#endif // SMOLRTSP_HEADER_MAP_H
