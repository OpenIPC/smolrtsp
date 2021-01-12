#include <smolrtsp/header.h>

#include "nala.h"

static void assert_pending(Slice99 input) {
    SmolRTSP_Header result;
    SmolRTSP_DeserializeResult res = SmolRTSP_Header_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(Slice99 input, SmolRTSP_Header expected) {
    SmolRTSP_Header result;
    SmolRTSP_DeserializeResult res = SmolRTSP_Header_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_Header_eq(result, expected));
}

static void assert_err(Slice99 input) {
    SmolRTSP_Header result;
    SmolRTSP_DeserializeResult res = SmolRTSP_Header_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultErr);
}

TEST(deserialize_header) {
    const Slice99 input = Slice99_from_str("Content-Length: 1556\r\n");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(Slice99_update_len(input, i));
    }

    assert_ok(
        input, (SmolRTSP_Header){
                   SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
                   Slice99_from_str("1556"),
               });

    assert_err(Slice99_from_str("~@~"));
}
