#include "../correctness.h"
#include <smolrtsp/deserializers/message_body.h>

#include <stdlib.h>

struct SmolRTSP_MessageBodyDeserializer {
    SmolRTSP_MessageBody inner;
    size_t bytes_read;
};

SmolRTSP_MessageBodyDeserializer *SmolRTSP_MessageBodyDeserializer_new(size_t content_length) {
    SmolRTSP_MessageBodyDeserializer *self;
    if ((self = (SmolRTSP_MessageBodyDeserializer *)malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->inner.len = content_length;
    self->inner.item_size = sizeof(char);
    self->bytes_read = 0;

    return self;
}

void SmolRTSP_MessageBodyDeserializer_free(SmolRTSP_MessageBodyDeserializer *self) {
    free(self);
}

SmolRTSP_MessageBody
SmolRTSP_MessageBodyDeserializer_inner(SmolRTSP_MessageBodyDeserializer *self) {
    precondition(self);

    return self->inner;
}

size_t SmolRTSP_MessageBodyDeserializer_bytes_read(SmolRTSP_MessageBodyDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_MessageBodyDeserializer_deserialize(
    SmolRTSP_MessageBodyDeserializer *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    if (Slice99_size(*data) < Slice99_size(self->inner)) {
        return SmolRTSP_DeserializeResultPending;
    }

    if (Slice99_size(self->inner) == 0) {
        self->inner = Slice99_empty(sizeof(char));
        return SmolRTSP_DeserializeResultOk;
    }

    self->inner.ptr = data->ptr;
    self->bytes_read = Slice99_size(self->inner);
    *data = Slice99_sub(*data, self->bytes_read, data->len);
    return SmolRTSP_DeserializeResultOk;
}
