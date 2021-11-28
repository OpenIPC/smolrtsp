#include <smolrtsp/message_body.h>

#include "parsing.h"

#include <assert.h>

SmolRTSP_DeserializeResult SmolRTSP_MessageBody_deserialize(
    SmolRTSP_MessageBody *restrict self, CharSlice99 *restrict data, size_t content_length) {
    assert(self);
    assert(data);

    if (CharSlice99_size(*data) < content_length) {
        return SmolRTSP_DeserializeResult_Pending;
    }

    if (0 == content_length) {
        *self = CharSlice99_empty();
        return SmolRTSP_DeserializeResult_Ok;
    }

    *self = CharSlice99_new(data->ptr, content_length);
    *data = CharSlice99_advance(*data, content_length);

    return SmolRTSP_DeserializeResult_Ok;
}
