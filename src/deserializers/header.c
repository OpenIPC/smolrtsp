#include "../parsing_aux.h"
#include <smolrtsp/deserializers/header.h>

#include <string.h>

struct SmolRTSP_HeaderDeserializer {
    char nothing;
} hollow_deserializer;

SmolRTSP_HeaderDeserializer *SmolRTSP_HeaderDeserializer_new(void) {
    return &hollow_deserializer;
}

void SmolRTSP_HeaderDeserializer_free(SmolRTSP_HeaderDeserializer *self) {}

SmolRTSP_DeserializeResult SmolRTSP_HeaderDeserializer_deserialize(
    SmolRTSP_HeaderDeserializer *restrict self, SmolRTSP_Header *restrict header, size_t size,
    const void *restrict data, size_t *restrict bytes_read) {
    SmolRTSP_Header parsed_header;

    SmolRTSP_DeserializeResult res = SmolRTSP_parse(
        sizeof(parsed_header), size, data, "%[^:] %[^" SMOLRTSP_CRLF "]%n", 3, &parsed_header.key,
        &parsed_header.value, bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        memcpy(header, &parsed_header, sizeof(parsed_header));
    }

    return res;
}
