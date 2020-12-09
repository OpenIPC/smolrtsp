#include "../deser_aux.h"
#include <smolrtsp/deserializers/header.h>
#include <smolrtsp/deserializers/header_map.h>

#include <assert.h>
#include <stdbool.h>
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

    self->bytes_read = self->inner.count = 0;

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
    SmolRTSP_HeaderMapDeserializer *restrict self, size_t size,
    const char data[restrict static size]) {
    assert(self);
    assert(data);

    while (true) {
        if (size < 2) {
            return SmolRTSP_DeserializeResultNeedMore;
        }

        if (data[0] == '\r' && data[1] == '\n') {
            self->bytes_read += 2;
            return SmolRTSP_DeserializeResultOk;
        }

        SmolRTSP_HeaderDeserializer *deser = SmolRTSP_HeaderDeserializer_new();
        if (deser == NULL) {
            return SmolRTSP_DeserializeResultErr;
        }

        MATCH(SmolRTSP_HeaderDeserializer_deserialize(deser, size, data));
        size_t bytes_read = SmolRTSP_HeaderDeserializer_bytes_read(deser);
        size -= bytes_read;
        data += bytes_read;
        self->bytes_read += bytes_read;
        self->inner.headers[self->inner.count] = SmolRTSP_HeaderDeserializer_inner(deser);
        self->inner.count++;

        SmolRTSP_HeaderDeserializer_free(deser);
    }
}
