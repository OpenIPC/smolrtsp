#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/deserializers/header.h>
#include <smolrtsp/deserializers/header_map.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct SmolRTSP_HeaderMapDeserializer {
    SmolRTSP_HeaderMap inner;
    size_t bytes_read;
};

SmolRTSP_HeaderMapDeserializer *
SmolRTSP_HeaderMapDeserializer_new(size_t size, SmolRTSP_Header headers[static size]) {
    precondition(size >= 0);
    precondition(headers);

    SmolRTSP_HeaderMapDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->inner = (SmolRTSP_HeaderMap){.headers = headers, .len = 0, .size = size};
    self->bytes_read = 0;

    return self;
}

void SmolRTSP_HeaderMapDeserializer_free(SmolRTSP_HeaderMapDeserializer *self) {
    free(self);
}

SmolRTSP_HeaderMap SmolRTSP_HeaderMapDeserializer_inner(SmolRTSP_HeaderMapDeserializer *self) {
    precondition(self);

    return self->inner;
}

size_t SmolRTSP_HeaderMapDeserializer_bytes_read(SmolRTSP_HeaderMapDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_HeaderMapDeserializer_deserialize(
    SmolRTSP_HeaderMapDeserializer *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    while (true) {
        if (data->len < 2) {
            return SmolRTSP_DeserializeResultPending;
        }

        if (((const char *)data->ptr)[0] == '\r' && ((const char *)data->ptr)[1] == '\n') {
            self->bytes_read += 2;
            *data = Slice99_sub(*data, 2, data->len);
            return SmolRTSP_DeserializeResultOk;
        }

        if (SmolRTSP_HeaderMap_is_full(self->inner)) {
            return SmolRTSP_DeserializeResultErr;
        }

        SmolRTSP_HeaderDeserializer *deser = SmolRTSP_HeaderDeserializer_new();
        if (deser == NULL) {
            return SmolRTSP_DeserializeResultErr;
        }

        MATCH(SmolRTSP_HeaderDeserializer_deserialize(deser, data));
        const size_t bytes_read = SmolRTSP_HeaderDeserializer_bytes_read(deser);
        self->bytes_read += bytes_read;
        self->inner.headers[self->inner.len] = SmolRTSP_HeaderDeserializer_inner(deser);
        self->inner.len++;

        SmolRTSP_HeaderDeserializer_free(deser);
    }
}
