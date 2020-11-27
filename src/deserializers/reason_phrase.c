#include "../parsing_aux.h"
#include <smolrtsp/deserializers/reason_phrase.h>

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
    return self->inner;
}

size_t SmolRTSP_ReasonPhraseDeserializer_bytes_read(SmolRTSP_ReasonPhraseDeserializer *self) {
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhraseDeserializer_deserialize(
    SmolRTSP_ReasonPhraseDeserializer *restrict self, size_t size, const void *restrict data) {
    SmolRTSP_ReasonPhrase phrase;
    size_t bytes_read;

    SmolRTSP_DeserializeResult res = SmolRTSP_parse(
        SMOLRTSP_REASON_PHRASE_SIZE, size, data, "%[^" SMOLRTSP_CRLF "]%n", 1, phrase, &bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        self->bytes_read += bytes_read;
        strncpy(self->inner.data, phrase.data, sizeof(phrase.data));
    }

    return res;
}
