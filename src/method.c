#include <smolrtsp/method.h>

#include "correctness.h"
#include "parsing.h"

SmolRTSP_DeserializeResult
SmolRTSP_Method_deserialize(SmolRTSP_Method *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    MATCH(SmolRTSP_match_whitespaces(data));
    Slice99 method = *data;
    MATCH(SmolRTSP_match_ident(data));
    method = Slice99_from_ptrdiff(method.ptr, data->ptr, sizeof(char));

    *self = method;

    return SmolRTSP_DeserializeResultOk;
}
