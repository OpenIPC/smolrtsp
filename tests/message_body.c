#include <smolrtsp/message_body.h>

#include <string.h>

#include "nala.h"

static void check(size_t size, Slice99 body, SmolRTSP_MessageBody expected) {
    SmolRTSP_MessageBody result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_MessageBody_deserialize(&result, &body, size);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(deserialize_message_body) {
    check(0, Slice99_from_str(""), Slice99_empty(sizeof(char)));
}
