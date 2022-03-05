#include <smolrtsp/types/reason_phrase.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_ParseResult SmolRTSP_ReasonPhrase_parse(
    SmolRTSP_ReasonPhrase *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(smolrtsp_match_whitespaces(input));
    CharSlice99 phrase = input;
    MATCH(smolrtsp_match_until_crlf(input));
    phrase = CharSlice99_from_ptrdiff(phrase.ptr, input.ptr - strlen("\r\n"));

    *self = phrase;

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_ReasonPhrase_eq(
    const SmolRTSP_ReasonPhrase *restrict lhs,
    const SmolRTSP_ReasonPhrase *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return CharSlice99_primitive_eq(*lhs, *rhs);
}
