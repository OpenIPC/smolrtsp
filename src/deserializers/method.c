#include "../parsing_aux.h"
#include <smolrtsp/deserializers/method.h>

#include <string.h>

struct SmolRTSP_MethodDeserializer {
    char nothing;
} hollow_deserializer;

SmolRTSP_MethodDeserializer *SmolRTSP_MethodDeserializer_new(void) {
    return &hollow_deserializer;
}

void SmolRTSP_MethodDeserializer_free(SmolRTSP_MethodDeserializer *self) {}

SmolRTSP_DeserializeResult SmolRTSP_MethodDeserializer_deserialize(
    SmolRTSP_MethodDeserializer *restrict self, SmolRTSP_Method *restrict method, size_t size,
    const void *restrict data, size_t *restrict bytes_read) {
    SmolRTSP_Method parsed_method;

    SmolRTSP_DeserializeResult res =
        SmolRTSP_parse(SMOLRTSP_METHOD_SIZE, size, data, "%s%n", 2, parsed_method, bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        strncpy((char *)method, parsed_method, sizeof(parsed_method));
    }

    return res;
}
