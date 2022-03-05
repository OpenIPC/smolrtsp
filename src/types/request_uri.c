#include <smolrtsp/types/request_uri.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_ParseResult SmolRTSP_RequestUri_parse(
    SmolRTSP_RequestUri *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(smolrtsp_match_whitespaces(input));
    CharSlice99 uri = input;
    MATCH(smolrtsp_match_non_whitespaces(input));
    uri = CharSlice99_from_ptrdiff(uri.ptr, input.ptr);

    *self = uri;

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_RequestUri_eq(
    const SmolRTSP_RequestUri *restrict lhs,
    const SmolRTSP_RequestUri *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return CharSlice99_primitive_eq(*lhs, *rhs);
}
