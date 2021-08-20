#include <smolrtsp/header_map.h>

#include "nala/nala.h"

static void assert_pending(CharSlice99 input) {
    SmolRTSP_HeaderMap result = SmolRTSP_HeaderMap_with_capacity(3);
    SmolRTSP_DeserializeResult res = SmolRTSP_HeaderMap_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultPending);
}

static void assert_ok(CharSlice99 input, SmolRTSP_HeaderMap expected) {
    SmolRTSP_HeaderMap result = SmolRTSP_HeaderMap_with_capacity(3);
    SmolRTSP_DeserializeResult res = SmolRTSP_HeaderMap_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_HeaderMap_eq(result, expected));
}

static void assert_err(CharSlice99 input) {
    SmolRTSP_HeaderMap result = SmolRTSP_HeaderMap_with_capacity(3);
    SmolRTSP_DeserializeResult res = SmolRTSP_HeaderMap_deserialize(&result, &input);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultErr);
}

#define HEADER_MAP                                                                                 \
    SmolRTSP_HeaderMap_from_array((SmolRTSP_Header[]){                                             \
        {                                                                                          \
            SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,                                                   \
            CharSlice99_from_str("10"),                                                            \
        },                                                                                         \
        {                                                                                          \
            SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,                                                  \
            CharSlice99_from_str("English"),                                                       \
        },                                                                                         \
        {                                                                                          \
            SMOLRTSP_HEADER_NAME_CONTENT_TYPE,                                                     \
            CharSlice99_from_str("application/octet-stream"),                                      \
        },                                                                                         \
    })

#define HEADER_MAP_STR                                                                             \
    "Content-Length: 10\r\nAccept-Language: English\r\nContent-Type: "                             \
    "application/octet-stream\r\n\r\n"

TEST(deserialize_header_map) {
    const SmolRTSP_HeaderMap expected = HEADER_MAP;

    const CharSlice99 input = CharSlice99_from_str(HEADER_MAP_STR);

    for (size_t i = 0; i < input.len - 1; i++) {
        assert_pending(CharSlice99_update_len(input, i));
    }

    assert_ok(input, expected);

    assert_err(CharSlice99_from_str("~29838"));
    assert_err(CharSlice99_from_str("Content-Length: 10\r\n38@@2: 10"));
}

TEST(serialize_header_map) {
    char buffer[500] = {0};
    const SmolRTSP_HeaderMap map = HEADER_MAP;

    SmolRTSP_HeaderMap_serialize(map, smolrtsp_char_buffer_writer, buffer);
    ASSERT_EQ(strcmp(buffer, HEADER_MAP_STR), 0);
}

TEST(find) {
    CharSlice99 content_length;
    const bool content_length_is_found =
        SmolRTSP_HeaderMap_find(HEADER_MAP, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH, &content_length);

    ASSERT(content_length_is_found);
    ASSERT(CharSlice99_primitive_eq(content_length, CharSlice99_from_str("10")));
}

TEST(key_is_present) {
    ASSERT(SmolRTSP_HeaderMap_key_is_present(HEADER_MAP, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH));
    ASSERT(!SmolRTSP_HeaderMap_key_is_present(HEADER_MAP, SMOLRTSP_HEADER_NAME_ALLOW));
}
