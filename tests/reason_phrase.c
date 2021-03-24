#include <smolrtsp/reason_phrase.h>

#include "nala/nala.h"

static void assert_pending(Slice99 input) {
    SmolRTSP_ReasonPhrase result;
    SmolRTSP_DeserializeResult res = SmolRTSP_ReasonPhrase_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(Slice99 input, SmolRTSP_ReasonPhrase expected) {
    SmolRTSP_ReasonPhrase result;
    SmolRTSP_DeserializeResult res = SmolRTSP_ReasonPhrase_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(deserialize_reason_phrase) {
    const Slice99 input = Slice99_from_str("Moved Temporarily\r\n");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(Slice99_update_len(input, i));
    }

    assert_ok(input, Slice99_from_str("Moved Temporarily"));
}
