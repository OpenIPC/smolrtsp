#include <smolrtsp/deserializers/header.h>
#include <smolrtsp/deserializers/header_map.h>

#include <stdlib.h>
#include <string.h>

struct SmolRTSP_HeaderMapDeserializer {
    SmolRTSP_HeaderMap inner;
    size_t bytes_read;
};

SmolRTSP_HeaderMapDeserializer *SmolRTSP_HeaderMapDeserializer_new(void) {
    SmolRTSP_HeaderMapDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;

    return self;
}

void SmolRTSP_HeaderMapDeserializer_free(SmolRTSP_HeaderMapDeserializer *self) {
    free(self);
}

SmolRTSP_HeaderMap SmolRTSP_HeaderMapDeserializer_inner(SmolRTSP_HeaderMapDeserializer *self) {
    return self->inner;
}

size_t SmolRTSP_HeaderMapDeserializer_bytes_read(SmolRTSP_HeaderMapDeserializer *self) {
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_HeaderMapDeserializer_deserialize(
    SmolRTSP_HeaderMapDeserializer *restrict self, size_t size, const void *restrict data) {
    SmolRTSP_HeaderDeserializer *header_deserializer = SmolRTSP_HeaderDeserializer_new();

    for (size_t i = self->inner.count; i < SMOLRTSP_HEADERS_COUNT; i++) {
        SmolRTSP_DeserializeResult res;
        if ((res = SmolRTSP_HeaderDeserializer_deserialize(header_deserializer, size, data)) ==
            SmolRTSP_DeserializeResultOk) {
            SmolRTSP_Header header = SmolRTSP_HeaderDeserializer_inner(header_deserializer);
            size_t bytes_read = SmolRTSP_HeaderDeserializer_bytes_read(header_deserializer);

            memcpy(&self->inner.headers[i], &header, sizeof(header));
            self->bytes_read += bytes_read;
        } else {
            return res;
        }
    }

    SmolRTSP_HeaderDeserializer_free(header_deserializer);
}
