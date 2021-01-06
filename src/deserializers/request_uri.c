#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/deserializers/request_uri.h>

#include <stdlib.h>
#include <string.h>

struct SmolRTSP_RequestURIDeserializer {
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

size_t SmolRTSP_RequestURIDeserializer_bytes_read(SmolRTSP_RequestURIDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RequestURIDeserializer_deserialize(
    SmolRTSP_RequestURIDeserializer *restrict self, SmolRTSP_RequestURI *restrict result,
    Slice99 *restrict data) {
    precondition(self);
    precondition(result);
    precondition(data);

    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    result->ptr = data->ptr;
    MATCH(SmolRTSP_match_non_whitespaces(data, &bytes_read));
    *result = Slice99_from_ptrdiff(result->ptr, data->ptr, sizeof(char));

    self->bytes_read += bytes_read;

    return SmolRTSP_DeserializeResultOk;
}
