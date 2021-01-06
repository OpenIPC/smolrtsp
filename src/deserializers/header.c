#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/deserializers/header.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct SmolRTSP_HeaderDeserializer {
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

size_t SmolRTSP_HeaderDeserializer_bytes_read(SmolRTSP_HeaderDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_HeaderDeserializer_deserialize(
    SmolRTSP_HeaderDeserializer *restrict self, SmolRTSP_Header *restrict result,
    Slice99 *restrict data) {
    precondition(self);
    precondition(result);
    precondition(data);

    SmolRTSP_Header header;
    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    header.key = *data;
    MATCH(SmolRTSP_match_header_name(data, &bytes_read));
    header.key = Slice99_from_ptrdiff(header.key.ptr, data->ptr, sizeof(char));

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    MATCH(SmolRTSP_match_char(data, &bytes_read, ':'));
    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));

    header.value = *data;

    MATCH(SmolRTSP_match_until_crlf(data, &bytes_read));
    header.value = Slice99_from_ptrdiff(header.value.ptr, data->ptr - strlen("\r\n"), sizeof(char));

    *result = header;
    self->bytes_read += bytes_read;

    return SmolRTSP_DeserializeResultOk;
}
