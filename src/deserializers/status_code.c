#include "../parsing_aux.h"
#include <smolrtsp/deserializers/status_code.h>

#include <inttypes.h>

struct SmolRTSP_StatusCodeDeserializer {
    char nothing;
} hollow_deserializer;

SmolRTSP_StatusCodeDeserializer *SmolRTSP_StatusCodeDeserializer_new(void) {
    return &hollow_deserializer;
}

void SmolRTSP_StatusCodeDeserializer_free(SmolRTSP_StatusCodeDeserializer *self) {}

SmolRTSP_DeserializeResult SmolRTSP_StatusCodeDeserializer_deserialize(
    SmolRTSP_StatusCodeDeserializer *restrict self, SmolRTSP_StatusCode *restrict code, size_t size,
    const void *restrict data, size_t *restrict bytes_read) {
    SmolRTSP_StatusCode parsed_code;
    SmolRTSP_DeserializeResult res =
        SmolRTSP_parse(6, size, data, "%" SCNuLEAST16, 1, &parsed_code);

    if (res == SmolRTSP_DeserializeResultOk) {
        *code = parsed_code;
    }

    return res;
}
