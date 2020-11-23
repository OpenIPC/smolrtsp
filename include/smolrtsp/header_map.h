#ifndef SMOLRTSP_HEADER_MAP_H
#define SMOLRTSP_HEADER_MAP_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/header.h>

#include <stddef.h>

typedef struct {
    size_t count;
    SmolRTSP_Header headers[SMOLRTSP_HEADERS_COUNT];
} SmolRTSP_HeaderMap;

void SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict map, SmolRTSP_UserWriter user_writer, void *user_cx);

SmolRTSP_DeserializeResult SmolRTSP_HeaderMap_deserialize(
    SmolRTSP_HeaderMap *restrict map, size_t size, const void *restrict data,
    size_t *restrict bytes_read);

#endif // SMOLRTSP_HEADER_MAP_H
