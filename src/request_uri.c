#include <smolrtsp/request_uri.h>

#include "correctness.h"
#include "parsing.h"

SmolRTSP_DeserializeResult
SmolRTSP_RequestURI_deserialize(SmolRTSP_RequestURI *restrict self, CharSlice99 *restrict data) {
    precondition(self);
    precondition(data);

    MATCH(SmolRTSP_match_whitespaces(data));
    CharSlice99 uri = *data;
    MATCH(SmolRTSP_match_non_whitespaces(data));
    uri = CharSlice99_from_ptrdiff(uri.ptr, data->ptr);

    *self = uri;

    return SmolRTSP_DeserializeResultOk;
}
