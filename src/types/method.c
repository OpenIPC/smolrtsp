#include <smolrtsp/types/method.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_ParseResult
SmolRTSP_Method_parse(SmolRTSP_Method *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(smolrtsp_match_whitespaces(input));
    CharSlice99 method = input;
    MATCH(smolrtsp_match_ident(input));
    method = CharSlice99_from_ptrdiff(method.ptr, input.ptr);

    *self = method;

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_Method_eq(
    const SmolRTSP_Method *restrict lhs, const SmolRTSP_Method *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return CharSlice99_primitive_eq(*lhs, *rhs);
}
