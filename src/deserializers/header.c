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
    SmolRTSP_HeaderDeserializer *restrict self, SmolRTSP_Slice data) {
    assert(self);
    assert(!SmolRTSP_Slice_is_null(data));

    const char *str = data.data;
    size_t size = data.size;

    SmolRTSP_Header header;
    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(&size, &str, &bytes_read));
    header.key.data = str;
    MATCH(SmolRTSP_match_header_name(&size, &str, &bytes_read));
    header.key.size = str - (const char *)header.key.data;

    MATCH(SmolRTSP_match_whitespaces(&size, &str, &bytes_read));
    EXPECT(size, str, bytes_read, ':');
    MATCH(SmolRTSP_match_whitespaces(&size, &str, &bytes_read));

    header.value.data = str;

    MATCH(SmolRTSP_match_crlf(&size, &str, &bytes_read));
    header.value.size = str - (const char *)header.value.data - 2;

    self->inner = header;
    self->bytes_read += bytes_read;

    return SmolRTSP_DeserializeResultOk;
}
