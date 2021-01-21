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
    const Slice99 input =
        Slice99_from_str("User-Agent: LibVLC/3.0.8 (LIVE555 Streaming Media v2018.02.18)\r\n");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(Slice99_update_len(input, i));
    }

    assert_ok(
        input, (SmolRTSP_Header){
                   SMOLRTSP_HEADER_NAME_USER_AGENT,
                   Slice99_from_str("LibVLC/3.0.8 (LIVE555 Streaming Media v2018.02.18)"),
               });

    assert_err(Slice99_from_str("~@~"));
}

TEST(serialize_header) {
    char buffer[200] = {0};

    const SmolRTSP_Header header = {
        SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
        Slice99_from_str("123"),
    };

    SmolRTSP_Header_serialize(header, smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(strcmp(buffer, "Content-Length: 123\r\n"), 0);
}
