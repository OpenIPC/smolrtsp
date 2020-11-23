#include <smolrtsp/header_map.h>

#include <string.h>

char *SmolRTSP_HeaderMap_find(const SmolRTSP_HeaderMap *restrict map, const char *restrict key) {
    for (size_t i = 0; i < map->count; i++) {
        if (strncmp(map->headers[i].key, key, SMOLRTSP_HEADER_KEY_SIZE) == 0) {
            return map->headers[i].value;
        }
    }

    return NULL;
}

void SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict map, SmolRTSP_UserWriter user_writer, void *user_cx) {
    for (size_t i = 0; i < map->count; i++) {
        SmolRTSP_Header_serialize(map->headers[i], user_writer, user_cx);
    }

    user_writer(strlen(SMOLRTSP_CRLF), SMOLRTSP_CRLF, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_HeaderMap_deserialize(
    SmolRTSP_HeaderMap *restrict map, size_t size, const void *restrict data,
    size_t *restrict bytes_read) {
    for (size_t i = map->count; i < SMOLRTSP_HEADERS_COUNT; i++) {
        size_t bytes_read_temp;

        switch (SmolRTSP_Header_deserialize(&map->headers[i], size, data, &bytes_read_temp)) {
        case SmolRTSP_DeserializeResultOk:
            *bytes_read += bytes_read_temp;
            break;
        case SmolRTSP_DeserializeResultErr:
            return SmolRTSP_DeserializeResultErr;
        case SmolRTSP_DeserializeResultNeedMore:
            return SmolRTSP_DeserializeResultNeedMore;
        }
    }
}
