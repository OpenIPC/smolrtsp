#include "../aux.h"
#include "../match.h"
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
    SmolRTSP_RequestURIDeserializer *restrict self, SmolRTSP_Slice *restrict data) {
    assert(self);
    assert(data);
    assert(!SmolRTSP_Slice_is_null(*data));

    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    const char *uri = data->ptr;
    MATCH(SmolRTSP_match_non_whitespaces(data, &bytes_read));
    const size_t uri_size = (const char *)data->ptr - uri;

    self->bytes_read += bytes_read;
    self->inner = SmolRTSP_Slice_new(uri, uri_size);

    return SmolRTSP_DeserializeResultOk;
}
