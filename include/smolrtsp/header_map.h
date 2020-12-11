/**
 * @file
 * @brief An RTSP header map.
 */

#ifndef SMOLRTSP_HEADER_MAP_H
#define SMOLRTSP_HEADER_MAP_H

#include <smolrtsp/header.h>

#include <stdbool.h>
#include <stddef.h>

/**
 * An RTSP header map.
 */
typedef struct {
    SmolRTSP_Header headers[SMOLRTSP_HEADERS_COUNT];
    size_t count;
} SmolRTSP_HeaderMap;

/**
 * Returns a value associated with @p key within @p map.
 *
 * @pre @p map shall not be `NULL`.
 * @pre @p key shall not be `NULL`.
 * @pre @p key shall be a null-terminated string.
 */
SmolRTSP_Slice SmolRTSP_HeaderMap_find(SmolRTSP_HeaderMap *restrict map, const char *restrict key);

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 */
void SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Compares @p lhs with @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre @p lhs shall not be `NULL`.
 * @pre @p rhs shall not be `NULL`.
 */
bool SmolRTSP_HeaderMap_eq(const SmolRTSP_HeaderMap *lhs, const SmolRTSP_HeaderMap *rhs);

#endif // SMOLRTSP_HEADER_MAP_H
