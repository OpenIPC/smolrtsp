#include <smolrtsp/deserializers/message_body.h>

#include <stdlib.h>

struct SmolRTSP_MessageBodyDeserializer {
    size_t content_length;
};

SmolRTSP_MessageBodyDeserializer *SmolRTSP_MessageBodyDeserializer_new(size_t content_length) {
    SmolRTSP_MessageBodyDeserializer *deserializer;
    if ((deserializer = (SmolRTSP_MessageBodyDeserializer *)malloc(sizeof(*deserializer))) ==
        NULL) {
        return NULL;
    }

    deserializer->content_length = content_length;
    return deserializer;
}

void SmolRTSP_MessageBodyDeserializer_free(SmolRTSP_MessageBodyDeserializer *deserializer) {
    free(deserializer);
}

SmolRTSP_DeserializeResult SmolRTSP_MessageBodyDeserializer_deserialize(
    SmolRTSP_MessageBodyDeserializer *restrict deserializer, SmolRTSP_MessageBody *restrict body,
    size_t size, const void *restrict data, size_t *restrict bytes_read) {
    if (size < deserializer->content_length) {
        return SmolRTSP_DeserializeResultNeedMore;
    }

    body->size = deserializer->content_length;
    body->data = data;
    return SmolRTSP_DeserializeResultOk;
}
