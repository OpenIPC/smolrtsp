#include "../aux.h"
#include "../matching.h"
#include <smolrtsp/deserializers/crlf.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SmolRTSP_CRLFDeserializer {
    SmolRTSP_CRLF inner;
    size_t bytes_read;
};

SmolRTSP_CRLFDeserializer *SmolRTSP_CRLFDeserializer_new(void) {
    SmolRTSP_CRLFDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;

    return self;
}

void SmolRTSP_CRLFDeserializer_free(SmolRTSP_CRLFDeserializer *self) {
    free(self);
}

SmolRTSP_CRLF SmolRTSP_CRLFDeserializer_inner(SmolRTSP_CRLFDeserializer *self) {
    assert(self);
    return self->inner;
}

size_t SmolRTSP_CRLFDeserializer_bytes_read(SmolRTSP_CRLFDeserializer *self) {
    assert(self);
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_CRLFDeserializer_deserialize(
    SmolRTSP_CRLFDeserializer *restrict self, SmolRTSP_Slice data) {
    assert(self);
    assert(!SmolRTSP_Slice_is_null(data));

    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_str(&data, &bytes_read, SMOLRTSP_CRLF));
    self->bytes_read += bytes_read;
    return SmolRTSP_DeserializeResultOk;
}
