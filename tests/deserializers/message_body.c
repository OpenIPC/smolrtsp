#include <smolrtsp/deserializers/message_body.h>

#include <string.h>

#include "../nala.h"

static void check(size_t size, const char *body, SmolRTSP_MessageBody expected) {
    SmolRTSP_MessageBodyDeserializer *deser = SmolRTSP_MessageBodyDeserializer_new(size);
    ASSERT_NE(deser, NULL);

    SmolRTSP_DeserializeResult res =
        SmolRTSP_MessageBodyDeserializer_deserialize(deser, SmolRTSP_Slice_new(body, size));
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);

    SmolRTSP_MessageBody inner = SmolRTSP_MessageBodyDeserializer_inner(deser);
    ASSERT(SmolRTSP_MessageBody_eq(&inner, &expected));

    SmolRTSP_MessageBodyDeserializer_free(deser);
}

TEST(test_deserializers_reason_phrase) {
    check(0, "", SmolRTSP_Slice_null());
}
