#include <smolrtsp/request_uri.h>

#include "nala.h"

static void assert_pending(Slice99 input) {
    SmolRTSP_RequestURI result;
    SmolRTSP_DeserializeResult res = SmolRTSP_RequestURI_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(Slice99 input, SmolRTSP_RequestURI expected) {
    SmolRTSP_RequestURI result;
    SmolRTSP_DeserializeResult res = SmolRTSP_RequestURI_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(deserialize_request_uri) {
    const Slice99 input = Slice99_from_str("http://example.com ");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(Slice99_update_len(input, i));
    }

    assert_ok(input, Slice99_from_str("http://example.com"));
}
