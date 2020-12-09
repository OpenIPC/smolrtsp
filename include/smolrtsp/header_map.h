/**
 * @file
 * @brief An RTSP header map.
 */

#ifndef SMOLRTSP_HEADER_MAP_H
#define SMOLRTSP_HEADER_MAP_H

#include <smolrtsp/header.h>

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    SmolRTSP_Header headers[SMOLRTSP_HEADERS_COUNT];
    size_t count;
} SmolRTSP_HeaderMap;

const char *SmolRTSP_HeaderMap_find(SmolRTSP_HeaderMap *restrict map, const char *restrict key);

void SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

bool SmolRTSP_HeaderMap_eq(const SmolRTSP_HeaderMap *lhs, const SmolRTSP_HeaderMap *rhs);

#endif // SMOLRTSP_HEADER_MAP_H
