#include "correctness.h"
#include "parsing.h"
#include <smolrtsp/message_body.h>

SmolRTSP_DeserializeResult SmolRTSP_MessageBody_deserialize(
    SmolRTSP_MessageBody *restrict self, Slice99 *restrict data, size_t content_length) {
    precondition(self);
    precondition(data);

    if (Slice99_size(*data) < content_length) {
        return SmolRTSP_DeserializeResultPending;
    }

    if (0 == content_length) {
        *self = Slice99_empty(sizeof(char));
        return SmolRTSP_DeserializeResultOk;
    }

    *self = Slice99_new(data->ptr, sizeof(char), content_length);
    *data = Slice99_advance(*data, content_length);
    
    return SmolRTSP_DeserializeResultOk;
}
