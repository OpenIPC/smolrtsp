#include <smolrtsp/message_body.h>

#include <string.h>

#include "../nala.h"

static void check(size_t size, const char *body, SmolRTSP_MessageBody expected) {
    Slice99 data = Slice99_new((char *)body, sizeof(char), size);
    SmolRTSP_MessageBody result;
    size_t bytes_read = 0;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_MessageBody_deserialize(&result, &data, &bytes_read, size);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(test_deserializers_reason_phrase) {
    check(0, "", Slice99_empty(sizeof(char)));
}
