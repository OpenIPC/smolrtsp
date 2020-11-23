#ifndef SMOLRTSP_REASON_PHRASE_H
#define SMOLRTSP_REASON_PHRASE_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/limits.h>

typedef char SmolRTSP_ReasonPhrase[SMOLRTSP_REASON_PHRASE_SIZE];

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhrase_deserialize(
    SmolRTSP_ReasonPhrase *restrict phrase, size_t size, const void *restrict data,
    size_t *restrict bytes_read);

#endif // SMOLRTSP_REASON_PHRASE_H
