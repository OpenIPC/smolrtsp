#include "../deser_aux.h"
#include <smolrtsp/deserializers/status_code.h>

#include <inttypes.h>
#include <stdlib.h>

struct SmolRTSP_StatusCodeDeserializer {
    SmolRTSP_StatusCode inner;
    size_t bytes_read;
};

SmolRTSP_StatusCodeDeserializer *SmolRTSP_StatusCodeDeserializer_new(void) {
    SmolRTSP_StatusCodeDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;
    return self;
}

void SmolRTSP_StatusCodeDeserializer_free(SmolRTSP_StatusCodeDeserializer *self) {
    free(self);
}

SmolRTSP_StatusCode SmolRTSP_StatusCodeDeserializer_inner(SmolRTSP_StatusCodeDeserializer *self) {
    return self->inner;
}

size_t SmolRTSP_StatusCodeDeserializer_bytes_read(SmolRTSP_StatusCodeDeserializer *self) {
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_StatusCodeDeserializer_deserialize(
    SmolRTSP_StatusCodeDeserializer *restrict self, size_t size,
    const char data[restrict static size]) {
    SmolRTSP_StatusCode code;
    int bytes_read;

    MATCH(SmolRTSP_parse(6, size, data, "%" SCNuLEAST16 "%n", 1, &code, &bytes_read));

    self->bytes_read = bytes_read;
    self->inner = code;

    return SmolRTSP_DeserializeResultOk;
}
