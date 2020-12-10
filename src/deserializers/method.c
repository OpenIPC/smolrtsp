#include "../aux.h"
#include "../deser_aux.h"
#include <smolrtsp/deserializers/method.h>

#include <stdlib.h>
#include <string.h>

struct SmolRTSP_MethodDeserializer {
    SmolRTSP_Method inner;
    size_t bytes_read;
};

SmolRTSP_MethodDeserializer *SmolRTSP_MethodDeserializer_new(void) {
    SmolRTSP_MethodDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;

    return self;
}

void SmolRTSP_MethodDeserializer_free(SmolRTSP_MethodDeserializer *self) {
    free(self);
}

SmolRTSP_Method SmolRTSP_MethodDeserializer_inner(SmolRTSP_MethodDeserializer *self) {
    return self->inner;
}

size_t SmolRTSP_MethodDeserializer_bytes_read(SmolRTSP_MethodDeserializer *self) {
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_MethodDeserializer_deserialize(
    SmolRTSP_MethodDeserializer *restrict self, size_t size,
    const char data[restrict static size]) {
    SmolRTSP_Method method;
    int bytes_read;

    MATCH(SmolRTSP_parse(
        SMOLRTSP_METHOD_SIZE, size, data, "%" STRINGIFY(SMOLRTSP_METHOD_SIZE) "s%n", 1, method.data,
        &bytes_read));

    self->bytes_read += bytes_read;
    strncpy(self->inner.data, method.data, sizeof(method.data));

    return SmolRTSP_DeserializeResultOk;
}
