#ifndef SMOLRTSP_DESERIALIZERS_REASON_PHRASE_H
#define SMOLRTSP_DESERIALIZERS_REASON_PHRASE_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/reason_phrase.h>

struct SmolRTSP_ReasonPhraseDeserializer;
typedef struct SmolRTSP_ReasonPhraseDeserializer SmolRTSP_ReasonPhraseDeserializer;

SmolRTSP_ReasonPhraseDeserializer *SmolRTSP_ReasonPhraseDeserializer_new(void);
void SmolRTSP_ReasonPhraseDeserializer_free(SmolRTSP_ReasonPhraseDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhraseDeserializer_deserialize(
    SmolRTSP_ReasonPhraseDeserializer *restrict self, SmolRTSP_ReasonPhrase *restrict phrase,
    size_t size, const void *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESERIALIZERS_REASON_PHRASE_H
