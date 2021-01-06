#include "../correctness.h"
#include <smolrtsp/deserializers/message_body.h>

#include <stdlib.h>

struct SmolRTSP_MessageBodyDeserializer {
    size_t bytes_read;
    size_t content_length;
};

SmolRTSP_MessageBodyDeserializer *SmolRTSP_MessageBodyDeserializer_new(size_t content_length) {
    SmolRTSP_MessageBodyDeserializer *self;
    if ((self = (SmolRTSP_MessageBodyDeserializer *)malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->content_length = content_length;
    self->bytes_read = 0;

    return self;
}

void SmolRTSP_MessageBodyDeserializer_free(SmolRTSP_MessageBodyDeserializer *self) {
    free(self);
}

size_t SmolRTSP_MessageBodyDeserializer_bytes_read(SmolRTSP_MessageBodyDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_MessageBodyDeserializer_deserialize(
    SmolRTSP_MessageBodyDeserializer *restrict self, SmolRTSP_MessageBody *restrict result,
    Slice99 *restrict data) {
    precondition(self);
    precondition(result);
    precondition(data);

    if (Slice99_size(*data) < self->content_length) {
        return SmolRTSP_DeserializeResultPending;
    }

    if (0 == self->content_length) {
        *result = Slice99_empty(sizeof(char));
        return SmolRTSP_DeserializeResultOk;
    }

    *result = Slice99_new(data->ptr, sizeof(char), self->content_length);
    self->bytes_read = self->content_length;
    *data = Slice99_sub(*data, self->bytes_read, data->len);
    return SmolRTSP_DeserializeResultOk;
}
