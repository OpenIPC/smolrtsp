#include <smolrtsp/message_body.h>

#include "nala.h"

static void assert_pending(Slice99 input, size_t content_length) {
    SmolRTSP_MessageBody result;
    SmolRTSP_DeserializeResult res =
        SmolRTSP_MessageBody_deserialize(&result, &input, content_length);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(Slice99 input, SmolRTSP_MessageBody expected, size_t content_length) {
    SmolRTSP_MessageBody result;
    SmolRTSP_DeserializeResult res =
        SmolRTSP_MessageBody_deserialize(&result, &input, content_length);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(deserialize_message_body) {
    const Slice99 input = Slice99_from_str(" 012345 ~ abc(^*%  D#NIN#3   ");
    const size_t content_length = input.len;

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(Slice99_update_len(input, i), content_length);
    }

    assert_ok(input, input, content_length);
    assert_ok(Slice99_from_str(""), Slice99_from_str(""), 0);
}
