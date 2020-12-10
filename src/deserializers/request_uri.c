#include "../aux.h"
#include "../deser_aux.h"
#include <smolrtsp/deserializers/request_uri.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct SmolRTSP_RequestURIDeserializer {
    SmolRTSP_RequestURI inner;
    size_t bytes_read;
};

SmolRTSP_RequestURIDeserializer *SmolRTSP_RequestURIDeserializer_new(void) {
    SmolRTSP_RequestURIDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;

    return self;
}

void SmolRTSP_RequestURIDeserializer_free(SmolRTSP_RequestURIDeserializer *self) {
    free(self);
}

SmolRTSP_RequestURI SmolRTSP_RequestURIDeserializer_inner(SmolRTSP_RequestURIDeserializer *self) {
    assert(self);
    return self->inner;
}

size_t SmolRTSP_RequestURIDeserializer_bytes_read(SmolRTSP_RequestURIDeserializer *self) {
    assert(self);
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RequestURIDeserializer_deserialize(
    SmolRTSP_RequestURIDeserializer *restrict self, size_t size, const char data[restrict]) {
    assert(self);
    assert(data);

    SmolRTSP_RequestURI uri;
    int bytes_read;

    MATCH(SmolRTSP_parse(
        SMOLRTSP_REQUEST_URI_SIZE, size, data, "%" STRINGIFY(SMOLRTSP_REQUEST_URI_SIZE) "s%n", 1,
        uri.data, &bytes_read));

    self->bytes_read += bytes_read;
    strncpy(self->inner.data, uri.data, sizeof(uri));

    return SmolRTSP_DeserializeResultOk;
}
