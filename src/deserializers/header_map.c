#include <smolrtsp/deserializers/header.h>
#include <smolrtsp/deserializers/header_map.h>

struct SmolRTSP_HeaderMapDeserializer {
    char nothing;
} hollow_deserializer;

SmolRTSP_HeaderMapDeserializer *SmolRTSP_HeaderMapDeserializer_new(void) {
    return &hollow_deserializer;
}

void SmolRTSP_HeaderMapDeserializer_free(SmolRTSP_HeaderMapDeserializer *deserializer) {}

SmolRTSP_DeserializeResult SmolRTSP_HeaderMapDeserializer_deserialize(
    SmolRTSP_HeaderMapDeserializer *restrict deserializer, SmolRTSP_HeaderMap *restrict map,
    size_t size, const void *restrict data, size_t *restrict bytes_read) {
    SmolRTSP_HeaderDeserializer *header_deserializer = SmolRTSP_HeaderDeserializer_new();

    for (size_t i = map->count; i < SMOLRTSP_HEADERS_COUNT; i++) {
        size_t bytes_read_temp;

        SmolRTSP_DeserializeResult res;
        if ((res = SmolRTSP_HeaderDeserializer_deserialize(
                 header_deserializer, &map->headers[i], size, data, &bytes_read_temp)) ==
            SmolRTSP_DeserializeResultOk) {
            *bytes_read += bytes_read_temp;
        } else {
            return res;
        }
    }

    SmolRTSP_HeaderDeserializer_free(header_deserializer);
}
