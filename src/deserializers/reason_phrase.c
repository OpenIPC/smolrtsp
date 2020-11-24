#include "../parsing_aux.h"
#include <smolrtsp/deserializers/reason_phrase.h>

#include <string.h>

struct SmolRTSP_ReasonPhraseDeserializer {
    char nothing;
} hollow_deserializer;

SmolRTSP_ReasonPhraseDeserializer *SmolRTSP_ReasonPhraseDeserializer_new(void) {
    return &hollow_deserializer;
}

void SmolRTSP_ReasonPhraseDeserializer_free(SmolRTSP_ReasonPhraseDeserializer *self) {}

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhraseDeserializer_deserialize(
    SmolRTSP_ReasonPhraseDeserializer *restrict self, SmolRTSP_ReasonPhrase *restrict phrase,
    size_t size, const void *restrict data, size_t *restrict bytes_read) {
    SmolRTSP_ReasonPhrase parsed_phrase;

    SmolRTSP_DeserializeResult res = SmolRTSP_parse(
        SMOLRTSP_REASON_PHRASE_SIZE, size, data, "%[^" SMOLRTSP_CRLF "]%n", 2, parsed_phrase,
        bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        strncpy((char *)phrase, parsed_phrase, sizeof(parsed_phrase));
    }

    return res;
}
