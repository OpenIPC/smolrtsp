#include <smolrtsp/status_code.h>

#include "nala.h"

static void assert_pending(Slice99 input) {
    SmolRTSP_StatusCode result;
    SmolRTSP_DeserializeResult res = SmolRTSP_StatusCode_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(Slice99 input, SmolRTSP_StatusCode expected) {
    SmolRTSP_StatusCode result;
    SmolRTSP_DeserializeResult res = SmolRTSP_StatusCode_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(result == expected);
}

static void assert_err(Slice99 input) {
    SmolRTSP_StatusCode result;
    SmolRTSP_DeserializeResult res = SmolRTSP_StatusCode_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultErr);
}

TEST(deserialize_status_code) {
    const Slice99 input = Slice99_from_str("404 ");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(Slice99_update_len(input, i));
    }

    assert_ok(input, SMOLRTSP_STATUS_CODE_NOT_FOUND);

    assert_err(Slice99_from_str("blah"));
    assert_err(Slice99_from_str("~ 2424 blah"));
}

TEST(serialize_status_code) {
    char buffer[20] = {0};

    SmolRTSP_StatusCode_serialize(
        SMOLRTSP_STATUS_CODE_NOT_FOUND, smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(strcmp(buffer, "404"), 0);
}
