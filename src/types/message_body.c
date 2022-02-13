#include <smolrtsp/types/message_body.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_ParseResult SmolRTSP_MessageBody_parse(
    SmolRTSP_MessageBody *restrict self, CharSlice99 input, size_t content_length) {
    assert(self);

    if (input.len < content_length) {
        return SmolRTSP_ParseResult_partial(0);
    }

    if (0 == content_length) {
        *self = CharSlice99_empty();
        return SmolRTSP_ParseResult_complete(0);
    }

    *self = CharSlice99_new(input.ptr, content_length);

    return SmolRTSP_ParseResult_complete(content_length);
}
