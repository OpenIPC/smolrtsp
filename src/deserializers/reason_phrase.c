#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/crlf.h>
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
    self->inner.item_size = sizeof(char);

    return self;
}

void SmolRTSP_ReasonPhraseDeserializer_free(SmolRTSP_ReasonPhraseDeserializer *self) {
    free(self);
}

SmolRTSP_ReasonPhrase
SmolRTSP_ReasonPhraseDeserializer_inner(SmolRTSP_ReasonPhraseDeserializer *self) {
    precondition(self);

    return self->inner;
}

size_t SmolRTSP_ReasonPhraseDeserializer_bytes_read(SmolRTSP_ReasonPhraseDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhraseDeserializer_deserialize(
    SmolRTSP_ReasonPhraseDeserializer *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    self->inner.ptr = data->ptr;
    MATCH(SmolRTSP_match_until_crlf(data, &bytes_read));
    self->inner =
        Slice99_from_ptrdiff(self->inner.ptr, data->ptr - strlen(SMOLRTSP_CRLF), sizeof(char));

    self->bytes_read = bytes_read;

    return SmolRTSP_DeserializeResultOk;
}
