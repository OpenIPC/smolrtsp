#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/deserializers/reason_phrase.h>

#include <stdlib.h>
#include <string.h>

struct SmolRTSP_ReasonPhraseDeserializer {
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

size_t SmolRTSP_ReasonPhraseDeserializer_bytes_read(SmolRTSP_ReasonPhraseDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhraseDeserializer_deserialize(
    SmolRTSP_ReasonPhraseDeserializer *restrict self, SmolRTSP_ReasonPhrase *restrict result,
    Slice99 *restrict data) {
    precondition(self);
    precondition(result);
    precondition(data);

    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    result->ptr = data->ptr;
    MATCH(SmolRTSP_match_until_crlf(data, &bytes_read));
    *result = Slice99_from_ptrdiff(result->ptr, data->ptr - strlen("\r\n"), sizeof(char));

    self->bytes_read = bytes_read;

    return SmolRTSP_DeserializeResultOk;
}
