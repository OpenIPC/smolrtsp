#include <smolrtsp/message_body.h>

#include "correctness.h"
#include "parsing.h"

SmolRTSP_DeserializeResult SmolRTSP_MessageBody_deserialize(
    SmolRTSP_MessageBody *restrict self, CharSlice99 *restrict data, size_t content_length) {
    precondition(self);
    precondition(data);

    if (CharSlice99_size(*data) < content_length) {
        return SmolRTSP_DeserializeResultPending;
    }

    if (0 == content_length) {
        *self = CharSlice99_empty();
        return SmolRTSP_DeserializeResultOk;
    }

    *self = CharSlice99_new(data->ptr, content_length);
    *data = CharSlice99_advance(*data, content_length);

    return SmolRTSP_DeserializeResultOk;
}
