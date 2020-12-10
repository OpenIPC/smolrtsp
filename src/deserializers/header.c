#include "../deser_aux.h"
#include <smolrtsp/deserializers/header.h>

#include <assert.h>
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
    assert(self);
    return self->inner;
}

size_t SmolRTSP_HeaderDeserializer_bytes_read(SmolRTSP_HeaderDeserializer *self) {
    assert(self);
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_HeaderDeserializer_deserialize(
    SmolRTSP_HeaderDeserializer *restrict self, size_t size,
    const char data[restrict static size]) {
    assert(self);
    assert(data);

    SmolRTSP_Header header;
    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(&size, &data, &bytes_read));
    header.key = data;
    MATCH(SmolRTSP_match_header_name(&size, &data, &bytes_read));
    header.key_len = data - header.key;

    MATCH(SmolRTSP_match_whitespaces(&size, &data, &bytes_read));
    EXPECT(size, data, bytes_read, ':');
    MATCH(SmolRTSP_match_whitespaces(&size, &data, &bytes_read));

    header.value = data;

    MATCH(SmolRTSP_match_crlf(&size, &data, &bytes_read));
    header.value_len = data - header.value - 2;

    self->inner = header;
    self->bytes_read += bytes_read;

    return SmolRTSP_DeserializeResultOk;
}
