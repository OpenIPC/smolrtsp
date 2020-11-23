#include "parsing_aux.h"
#include <smolrtsp/reason_phrase.h>

#include <string.h>

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhrase_deserialize(
    SmolRTSP_ReasonPhrase *restrict phrase, size_t size, const void *restrict data) {
    SmolRTSP_ReasonPhrase parsed_phrase;
    SmolRTSP_DeserializeResult res = SmolRTSP_parse(
        SMOLRTSP_REASON_PHRASE_SIZE, size, data, "%[^" SMOLRTSP_CRLF "]", 1, parsed_phrase);

    if (res == SmolRTSP_DeserializeResultOk) {
        strncpy((char *)phrase, parsed_phrase, sizeof(parsed_phrase));
    }

    return res;
}
