#include <smolrtsp/header_map.h>

#include <greatest.h>

#include <assert.h>

static enum greatest_test_res assert_pending(CharSlice99 input) {
    const size_t headers_len = 3;
    SmolRTSP_Header *headers = malloc(sizeof headers[0] * headers_len);
    assert(headers);
    SmolRTSP_HeaderMap result = {.headers = headers, .len = 0, .capacity = headers_len};
    SmolRTSP_ParseResult res = SmolRTSP_HeaderMap_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_partial(res));
    free(headers);
    PASS();
}

static enum greatest_test_res assert_ok(CharSlice99 input, SmolRTSP_HeaderMap expected) {
    const size_t headers_len = 3;
    SmolRTSP_Header *headers = malloc(sizeof headers[0] * headers_len);
    assert(headers);
    SmolRTSP_HeaderMap result = {.headers = headers, .len = 0, .capacity = headers_len};
    SmolRTSP_ParseResult res = SmolRTSP_HeaderMap_parse(&result, input);
    ASSERT(SmolRTSP_ParseResult_is_complete(res));
    ASSERT(SmolRTSP_HeaderMap_eq(result, expected));
    free(headers);
    PASS();
}

static enum greatest_test_res assert_err(CharSlice99 input) {
    const size_t headers_len = 3;
    SmolRTSP_Header *headers = malloc(sizeof headers[0] * headers_len);
    assert(headers);
    SmolRTSP_HeaderMap result = {.headers = headers, .len = 0, .capacity = headers_len};
    SmolRTSP_ParseResult res = SmolRTSP_HeaderMap_parse(&result, input);
    ASSERT(MATCHES(res, SmolRTSP_ParseResult_Failure));
    free(headers);
    PASS();
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

TEST parse_header_map(void) {
    const SmolRTSP_HeaderMap expected = HEADER_MAP;

    const CharSlice99 input = CharSlice99_from_str(HEADER_MAP_STR);

    for (size_t i = 0; i < input.len - 1; i++) {
        CHECK_CALL(assert_pending(CharSlice99_update_len(input, i)));
    }

    CHECK_CALL(assert_ok(input, expected));

    CHECK_CALL(assert_err(CharSlice99_from_str("~29838")));
    CHECK_CALL(assert_err(CharSlice99_from_str("Content-Length: 10\r\n38@@2: 10")));

    PASS();
}

TEST serialize_header_map(void) {
    char buffer[500] = {0};
    const SmolRTSP_HeaderMap map = HEADER_MAP;

    SmolRTSP_HeaderMap_serialize(map, smolrtsp_char_buffer_writer, buffer);
    ASSERT_EQ(strcmp(buffer, HEADER_MAP_STR), 0);

    PASS();
}

TEST find(void) {
    CharSlice99 content_length;
    const bool content_length_is_found =
        SmolRTSP_HeaderMap_find(HEADER_MAP, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH, &content_length);

    ASSERT(content_length_is_found);
    ASSERT(CharSlice99_primitive_eq(content_length, CharSlice99_from_str("10")));

    PASS();
}

TEST key_is_present(void) {
    ASSERT(SmolRTSP_HeaderMap_key_is_present(HEADER_MAP, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH));
    ASSERT(!SmolRTSP_HeaderMap_key_is_present(HEADER_MAP, SMOLRTSP_HEADER_NAME_ALLOW));

    PASS();
}

SUITE(header_map) {
    RUN_TEST(parse_header_map);
    RUN_TEST(serialize_header_map);
    RUN_TEST(find);
    RUN_TEST(key_is_present);
}
