#include "../parsing_aux.h"
#include <smolrtsp/deserializers/request_uri.h>

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
    return self->inner;
}

size_t SmolRTSP_RequestURIDeserializer_bytes_read(SmolRTSP_RequestURIDeserializer *self) {
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RequestURIDeserializer_deserialize(
    SmolRTSP_RequestURIDeserializer *restrict self, size_t size, const void *restrict data) {
    SmolRTSP_RequestURI uri;

    SmolRTSP_DeserializeResult res =
        SmolRTSP_parse(SMOLRTSP_REQUEST_URI_SIZE, size, data, "%s%n", 2, uri, &self->bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        strncpy(self->inner.data, uri.data, sizeof(uri));
    }

    return res;
}
