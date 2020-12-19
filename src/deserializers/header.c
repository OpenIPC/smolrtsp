#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/crlf.h>
#include <smolrtsp/deserializers/header.h>

#include <stdbool.h>
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
    precondition(self);

    return self->inner;
}

size_t SmolRTSP_HeaderDeserializer_bytes_read(SmolRTSP_HeaderDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_HeaderDeserializer_deserialize(
    SmolRTSP_HeaderDeserializer *restrict self, SmolRTSP_Slice *restrict data) {
    precondition(self);
    precondition(!SmolRTSP_Slice_is_null(*data));

    SmolRTSP_Header header;
    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    header.key.ptr = data->ptr;
    MATCH(SmolRTSP_match_header_name(data, &bytes_read));
    header.key.size = (const char *)data->ptr - (const char *)header.key.ptr;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    MATCH(SmolRTSP_match_char(data, &bytes_read, ':'));
    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));

    header.value.ptr = data->ptr;

    MATCH(SmolRTSP_match_until_crlf(data, &bytes_read));
    header.value.size =
        (const char *)data->ptr - (const char *)header.value.ptr - strlen(SMOLRTSP_CRLF);

    self->inner = header;
    self->bytes_read += bytes_read;

    return SmolRTSP_DeserializeResultOk;
}
