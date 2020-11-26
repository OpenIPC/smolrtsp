#include "../parsing_aux.h"
#include <smolrtsp/deserializers/header.h>

#include <stdlib.h>
#include <string.h>

struct SmolRTSP_HeaderDeserializer {
    SmolRTSP_Header inner;
    size_t bytes_read;
};

SmolRTSP_HeaderDeserializer *SmolRTSP_HeaderDeserializer_new(void) {
    SmolRTSP_HeaderDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;

    return self;
}

void SmolRTSP_HeaderDeserializer_free(SmolRTSP_HeaderDeserializer *self) {
    free(self);
}

SmolRTSP_Header SmolRTSP_HeaderDeserializer_inner(SmolRTSP_HeaderDeserializer *self) {
    return self->inner;
}

size_t SmolRTSP_HeaderDeserializer_bytes_read(SmolRTSP_HeaderDeserializer *self) {
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_HeaderDeserializer_deserialize(
    SmolRTSP_HeaderDeserializer *restrict self, size_t size, const void *restrict data) {
    SmolRTSP_Header header;
    size_t bytes_read;

    SmolRTSP_DeserializeResult res = SmolRTSP_parse(
        sizeof(header), size, data, "%[^:] %[^" SMOLRTSP_CRLF "]%n", 3, &header.key, &header.value,
        bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        self->bytes_read += bytes_read;
        memcpy(&self->inner, &header, sizeof(header));
    }

    return res;
}
