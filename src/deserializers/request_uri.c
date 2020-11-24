#include "../parsing_aux.h"
#include <smolrtsp/deserializers/request_uri.h>

#include <string.h>

struct SmolRTSP_RequestURIDeserializer {
    char nothing;
} hollow_deserializer;

SmolRTSP_RequestURIDeserializer *SmolRTSP_RequestURIDeserializer_new(void) {
    return &hollow_deserializer;
}

void SmolRTSP_RequestURIDeserializer_free(SmolRTSP_RequestURIDeserializer *self) {}

SmolRTSP_DeserializeResult SmolRTSP_RequestURIDeserializer_deserialize(
    SmolRTSP_RequestURIDeserializer *restrict self, SmolRTSP_RequestURI *restrict uri, size_t size,
    const void *restrict data, size_t *restrict bytes_read) {
    SmolRTSP_RequestURI parsed_uri;

    SmolRTSP_DeserializeResult res =
        SmolRTSP_parse(SMOLRTSP_REQUEST_URI_SIZE, size, data, "%s%n", 2, parsed_uri, bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        strncpy(*uri, parsed_uri, sizeof(parsed_uri));
    }

    return res;
}
