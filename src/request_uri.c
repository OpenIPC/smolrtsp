#include "correctness.h"
#include "parsing.h"
#include <smolrtsp/request_uri.h>

SmolRTSP_DeserializeResult
SmolRTSP_RequestURI_deserialize(SmolRTSP_RequestURI *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    MATCH(SmolRTSP_match_whitespaces(data));
    Slice99 uri = *data;
    MATCH(SmolRTSP_match_non_whitespaces(data));
    uri = Slice99_from_ptrdiff(uri.ptr, data->ptr, sizeof(char));

    *self = uri;

    return SmolRTSP_DeserializeResultOk;
}
