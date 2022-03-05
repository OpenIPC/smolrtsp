/**
 * @file
 * @brief An RTSP header map.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/error.h>
#include <smolrtsp/types/header.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <unistd.h>

#include <slice99.h>

/**
 * Creates new #SmolRTSP_HeaderMap from an array expression of #SmolRTSP_Header.
 */
#define SmolRTSP_HeaderMap_from_array(...)                                     \
    ((SmolRTSP_HeaderMap){                                                     \
        .headers = __VA_ARGS__,                                                \
        .len = SLICE99_ARRAY_LEN((SmolRTSP_Header[])__VA_ARGS__),              \
    })

/**
 * The maximum number of headers in #SmolRTSP_HeaderMap.headers.
 */
#define SMOLRTSP_HEADER_MAP_CAPACITY 32

/**
 * An RTSP header map.
 */
typedef struct {
    /**
     * The pointer to an array of headers;
     */
    SmolRTSP_Header headers[SMOLRTSP_HEADER_MAP_CAPACITY];

    /**
     * The count of elements currently in #headers.
     */
    size_t len;
} SmolRTSP_HeaderMap;

/**
 * Returns an empty header map suitable for further parsing.
 */
SmolRTSP_HeaderMap SmolRTSP_HeaderMap_empty(void) SMOLRTSP_PRIV_MUST_USE;

/**
 * Finds a value associated with @p key within @p self.
 *
 * If @p key has been found within @p self, this function assigns @p value to
 * this key (no copying occurs) and returns `true`. Otherwise, returns `false`
 * and @p value remains unchanged.
 *
 * @param[in] self The header map to be searched for @p key.
 * @param[in] key The key to be searched in @p self.
 * @param[out] value The header value to be assigned, if found. If `NULL`, no
 * assignment is performed.
 *
 * @pre `self != NULL`
 */
bool SmolRTSP_HeaderMap_find(
    const SmolRTSP_HeaderMap *restrict self, CharSlice99 key,
    CharSlice99 *restrict value) SMOLRTSP_PRIV_MUST_USE;

/**
 * Returns whether @p key is present in @p self.
 *
 * @pre `self != NULL`
 */
bool SmolRTSP_HeaderMap_contains_key(
    const SmolRTSP_HeaderMap *restrict self,
    CharSlice99 key) SMOLRTSP_PRIV_MUST_USE;

/**
 * Appends a new header to a header map.
 *
 * @param[out] self The header map to modify.
 * @param[in] h The new header to be appended.
 *
 * @pre `self != NULL`
 * @pre `!SmolRTSP_HeaderMap_is_full(self)`
 */
void SmolRTSP_HeaderMap_append(
    SmolRTSP_HeaderMap *restrict self, SmolRTSP_Header h);

/**
 * Serialises @p self into @p w.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The writer to be provided with serialised data.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `self != NULL`
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict self,
    SmolRTSP_Writer w) SMOLRTSP_PRIV_MUST_USE;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_HeaderMap_parse(
    SmolRTSP_HeaderMap *restrict self,
    CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_HeaderMap_eq(
    const SmolRTSP_HeaderMap *restrict lhs,
    const SmolRTSP_HeaderMap *restrict rhs) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests whether @p self is full (no more space left for an additional header)
 * or not.
 *
 * @return `true` if @p self is full, `false` otherwise.
 *
 * @pre `self != NULL`
 */
bool SmolRTSP_HeaderMap_is_full(const SmolRTSP_HeaderMap *restrict self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Attempts to parse a header.
 *
 * @param[in] headers The header map to search @p key in.
 * @param[in] key The header key to search for.
 * @param[in] fmt The `scanf-`like format string.
 *
 * @return The number of scanned parameters or -1 if @p key is not found.
 *
 * @pre `headers != NULL`
 * @pre `fmt != NULL`
 */
int smolrtsp_scanf_header(
    const SmolRTSP_HeaderMap *restrict headers, CharSlice99 key,
    const char *restrict fmt, ...) SMOLRTSP_PRIV_MUST_USE
    SMOLRTSP_PRIV_GCC_ATTR(format(scanf, 3, 4));
