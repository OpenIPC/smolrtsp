#include <smolrtsp/request_uri.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_ParseResult
SmolRTSP_RequestURI_parse(SmolRTSP_RequestURI *restrict self, CharSlice99 *restrict data) {
    assert(self);
    assert(data);

    MATCH(smolrtsp_match_whitespaces(data));
    CharSlice99 uri = *data;
    MATCH(smolrtsp_match_non_whitespaces(data));
    uri = CharSlice99_from_ptrdiff(uri.ptr, data->ptr);

    *self = uri;

    return SmolRTSP_ParseResult_Ok;
}
