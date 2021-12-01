#include <smolrtsp/method.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_ParseResult
SmolRTSP_Method_parse(SmolRTSP_Method *restrict self, CharSlice99 *restrict data) {
    assert(self);
    assert(data);

    MATCH(smolrtsp_match_whitespaces(data));
    CharSlice99 method = *data;
    MATCH(smolrtsp_match_ident(data));
    method = CharSlice99_from_ptrdiff(method.ptr, data->ptr);

    *self = method;

    return SmolRTSP_ParseResult_Ok;
}
