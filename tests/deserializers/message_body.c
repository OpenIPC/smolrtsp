#include <smolrtsp/deserializers/message_body.h>

#include <string.h>

#include "../nala.h"

static void check(size_t size, const char *body, SmolRTSP_MessageBody expected) {
    SmolRTSP_MessageBodyDeserializer *deser = SmolRTSP_MessageBodyDeserializer_new(size);
    ASSERT_NE(deser, NULL);

    Slice99 data = Slice99_new((char *)body, sizeof(char), size);
    SmolRTSP_MessageBody result;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_MessageBodyDeserializer_deserialize(deser, &result, &data);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));

    SmolRTSP_MessageBodyDeserializer_free(deser);
}

TEST(test_deserializers_reason_phrase) {
    check(0, "", Slice99_empty(sizeof(char)));
}
