#include "../aux.h"
#include "../deser_aux.h"
#include <smolrtsp/deserializers/reason_phrase.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct SmolRTSP_ReasonPhraseDeserializer {
    SmolRTSP_ReasonPhrase inner;
    size_t bytes_read;
};

SmolRTSP_ReasonPhraseDeserializer *SmolRTSP_ReasonPhraseDeserializer_new(void) {
    SmolRTSP_ReasonPhraseDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;

    return self;
}

void SmolRTSP_ReasonPhraseDeserializer_free(SmolRTSP_ReasonPhraseDeserializer *self) {
    free(self);
}

SmolRTSP_ReasonPhrase
SmolRTSP_ReasonPhraseDeserializer_inner(SmolRTSP_ReasonPhraseDeserializer *self) {
    assert(self);
    return self->inner;
}

size_t SmolRTSP_ReasonPhraseDeserializer_bytes_read(SmolRTSP_ReasonPhraseDeserializer *self) {
    assert(self);
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhraseDeserializer_deserialize(
    SmolRTSP_ReasonPhraseDeserializer *restrict self, size_t size,
    const char data[restrict static size]) {
    assert(self);
    assert(data);

    SmolRTSP_ReasonPhrase phrase;
    int bytes_read;

    MATCH(SmolRTSP_parse(
        SMOLRTSP_REASON_PHRASE_SIZE, size, data,
        "%" STRINGIFY(SMOLRTSP_REASON_PHRASE_SIZE) "[^" CRLF "]%n", 1, phrase.data, &bytes_read));

    self->bytes_read += bytes_read;
    strncpy(self->inner.data, phrase.data, sizeof(phrase.data));

    return SmolRTSP_DeserializeResultOk;
}
