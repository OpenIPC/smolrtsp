/**
 * @file
 * @brief An RTSP reason phrase deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_REASON_PHRASE_H
#define SMOLRTSP_DESERIALIZERS_REASON_PHRASE_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/reason_phrase.h>

struct SmolRTSP_ReasonPhraseDeserializer;
typedef struct SmolRTSP_ReasonPhraseDeserializer SmolRTSP_ReasonPhraseDeserializer;

SmolRTSP_ReasonPhraseDeserializer *SmolRTSP_ReasonPhraseDeserializer_new(void);
void SmolRTSP_ReasonPhraseDeserializer_free(SmolRTSP_ReasonPhraseDeserializer *self);

SmolRTSP_ReasonPhrase
SmolRTSP_ReasonPhraseDeserializer_inner(SmolRTSP_ReasonPhraseDeserializer *self);
size_t SmolRTSP_ReasonPhraseDeserializer_bytes_read(SmolRTSP_ReasonPhraseDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhraseDeserializer_deserialize(
    SmolRTSP_ReasonPhraseDeserializer *restrict self, size_t size, const char data[restrict]);

#endif // SMOLRTSP_DESERIALIZERS_REASON_PHRASE_H
