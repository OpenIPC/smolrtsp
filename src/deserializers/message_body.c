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

    self->inner.size = content_length;
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
    SmolRTSP_MessageBodyDeserializer *restrict self, SmolRTSP_Slice *restrict data) {
    precondition(self);
    precondition(data);
    precondition(!SmolRTSP_Slice_is_null(*data));

    if (data->size < self->inner.size) {
        return SmolRTSP_DeserializeResultNeedMore;
    }

    if (self->inner.size == 0) {
        self->inner = SmolRTSP_Slice_null();
        return SmolRTSP_DeserializeResultOk;
    }

    self->inner.ptr = data->ptr;
    self->bytes_read = self->inner.size;
    *data = SmolRTSP_Slice_advance(*data, self->bytes_read);
    return SmolRTSP_DeserializeResultOk;
}
