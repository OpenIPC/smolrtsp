#include <smolrtsp/deserializers/message_body.h>

#include <stdlib.h>

struct SmolRTSP_MessageBodyDeserializer {
    size_t content_length;
};

SmolRTSP_MessageBodyDeserializer *SmolRTSP_MessageBodyDeserializer_new(size_t content_length) {
    SmolRTSP_MessageBodyDeserializer *self;
    if ((self = (SmolRTSP_MessageBodyDeserializer *)malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->content_length = content_length;
    return self;
}

void SmolRTSP_MessageBodyDeserializer_free(SmolRTSP_MessageBodyDeserializer *self) {
    free(self);
}

SmolRTSP_DeserializeResult SmolRTSP_MessageBodyDeserializer_deserialize(
    SmolRTSP_MessageBodyDeserializer *restrict self, SmolRTSP_MessageBody *restrict body,
    size_t size, const void *restrict data, size_t *restrict bytes_read) {
    if (size < self->content_length) {
        return SmolRTSP_DeserializeResultNeedMore;
    }

    body->size = self->content_length;
    body->data = data;
    return SmolRTSP_DeserializeResultOk;
}
