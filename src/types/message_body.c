#include <smolrtsp/types/message_body.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_ParseResult SmolRTSP_MessageBody_parse(
    SmolRTSP_MessageBody *restrict self, CharSlice99 input,
    size_t content_length) {
    assert(self);

    if (input.len < content_length) {
        return SmolRTSP_ParseResult_partial();
    }

    if (0 == content_length) {
        *self = CharSlice99_empty();
        const size_t offset = 0;
        return SmolRTSP_ParseResult_complete(offset);
    }

    *self = CharSlice99_new(input.ptr, content_length);

    return SmolRTSP_ParseResult_complete(content_length);
}

SmolRTSP_MessageBody SmolRTSP_MessageBody_empty(void) {
    return CharSlice99_empty();
}

bool SmolRTSP_MessageBody_eq(
    const SmolRTSP_MessageBody *restrict lhs,
    const SmolRTSP_MessageBody *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return CharSlice99_primitive_eq(*lhs, *rhs);
}
