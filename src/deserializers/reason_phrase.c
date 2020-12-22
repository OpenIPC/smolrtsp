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
    SmolRTSP_ReasonPhraseDeserializer *restrict self, SmolRTSP_Slice *restrict data) {
    precondition(self);
    precondition(data);
    precondition(!SmolRTSP_Slice_is_null(*data));

    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    const char *phrase = data->ptr;
    MATCH(SmolRTSP_match_until_crlf(data, &bytes_read));
    const size_t phrase_size = (const char *)data->ptr - phrase - strlen(SMOLRTSP_CRLF);

    self->bytes_read = bytes_read;
    self->inner = SmolRTSP_Slice_new(phrase, phrase_size);

    return SmolRTSP_DeserializeResultOk;
}
