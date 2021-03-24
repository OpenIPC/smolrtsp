#include <smolrtsp/header_map.h>

#include "nala/nala.h"

static void assert_pending(Slice99 input) {
    SmolRTSP_HeaderMap result = SmolRTSP_HeaderMap_with_capacity(3);
    SmolRTSP_DeserializeResult res = SmolRTSP_HeaderMap_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(Slice99 input, SmolRTSP_HeaderMap expected) {
    SmolRTSP_HeaderMap result = SmolRTSP_HeaderMap_with_capacity(3);
    SmolRTSP_DeserializeResult res = SmolRTSP_HeaderMap_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_HeaderMap_eq(result, expected));
}

static void assert_err(Slice99 input) {
    SmolRTSP_HeaderMap result = SmolRTSP_HeaderMap_with_capacity(3);
    SmolRTSP_DeserializeResult res = SmolRTSP_HeaderMap_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultErr);
}

TEST(deserialize_header_map) {
    const SmolRTSP_HeaderMap expected = SmolRTSP_HeaderMap_from_array((SmolRTSP_Header[]){
        {
            SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
            Slice99_from_str("10"),
        },
        {
            SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
            Slice99_from_str("English"),
        },
        {
            SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
            Slice99_from_str("application/octet-stream"),
        },
    });

    const Slice99 input = Slice99_from_str(
        "Content-Length: 10\r\nAccept-Language: English\r\nContent-Type: "
        "application/octet-stream\r\n\r\n");

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(Slice99_update_len(input, i));
    }

    assert_ok(input, expected);

    assert_err(Slice99_from_str("~29838"));
    assert_err(Slice99_from_str("Content-Length: 10\r\n38@@2: 10"));
}

TEST(serialize_header_map) {
    char buffer[500] = {0};

    const SmolRTSP_HeaderMap map = SmolRTSP_HeaderMap_from_array((SmolRTSP_Header[]){
        {
            SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
            Slice99_from_str("123"),
        },
        {
            SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
            Slice99_from_str("application/octet-stream"),
        },
    });

    SmolRTSP_HeaderMap_serialize(map, smolrtsp_char_buffer_writer, buffer);

    ASSERT_EQ(
        strcmp(buffer, "Content-Length: 123\r\nContent-Type: application/octet-stream\r\n\r\n"), 0);
}
