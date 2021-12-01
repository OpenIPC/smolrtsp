#include <smolrtsp/reason_phrase.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhrase_deserialize(
    SmolRTSP_ReasonPhrase *restrict self, CharSlice99 *restrict data) {
    assert(self);
    assert(data);

    MATCH(smolrtsp_match_whitespaces(data));
    CharSlice99 phrase = *data;
    MATCH(smolrtsp_match_until_crlf(data));
    phrase = CharSlice99_from_ptrdiff(phrase.ptr, data->ptr - strlen("\r\n"));

    *self = phrase;

    return SmolRTSP_DeserializeResult_Ok;
}
