#include "correctness.h"
#include "parsing.h"
#include <smolrtsp/reason_phrase.h>

SmolRTSP_DeserializeResult
SmolRTSP_ReasonPhrase_deserialize(SmolRTSP_ReasonPhrase *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    MATCH(SmolRTSP_match_whitespaces(data));
    Slice99 phrase = *data;
    MATCH(SmolRTSP_match_until_crlf(data));
    phrase = Slice99_from_ptrdiff(phrase.ptr, data->ptr - strlen("\r\n"), sizeof(char));

    *self = phrase;

    return SmolRTSP_DeserializeResultOk;
}
