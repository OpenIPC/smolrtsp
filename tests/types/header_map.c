#include <smolrtsp/types/header_map.h>

#include <stdlib.h>

#define TEST_PARSE_INIT_TYPE(result) result = SmolRTSP_HeaderMap_empty()

#include "test_util.h"
#include <greatest.h>

#include <slice99.h>

DEF_TEST_PARSE(SmolRTSP_HeaderMap)

#define HEADER_MAP                                                             \
    SmolRTSP_HeaderMap_from_array({                                            \
        {SMOLRTSP_HEADER_CONTENT_LENGTH, CharSlice99_from_str("10")},          \
        {SMOLRTSP_HEADER_ACCEPT_LANGUAGE, CharSlice99_from_str("English")},    \
        {SMOLRTSP_HEADER_CONTENT_TYPE,                                         \
         CharSlice99_from_str("application/octet-stream")},                    \
    })

#define HEADER_MAP_STR                                                         \
    "Content-Length: 10\r\nAccept-Language: English\r\nContent-Type: "         \
    "application/octet-stream\r\n\r\n"

TEST parse_header_map(void) {
    TEST_PARSE(HEADER_MAP_STR, HEADER_MAP);

    SmolRTSP_HeaderMap result = SmolRTSP_HeaderMap_empty();

    ASSERT(SmolRTSP_ParseResult_is_failure(SmolRTSP_HeaderMap_parse(
        &result, CharSlice99_from_str("~29838\r\n\r\n"))));
    ASSERT(SmolRTSP_ParseResult_is_failure(SmolRTSP_HeaderMap_parse(
        &result,
        CharSlice99_from_str("Content-Length: 10\r\n38@@2: 10\r\n\r\n"))));

    PASS();
}

TEST serialize_header_map(void) {
    char buffer[500] = {0};
    const SmolRTSP_HeaderMap map = HEADER_MAP;

    const ssize_t ret =
        SmolRTSP_HeaderMap_serialize(&map, smolrtsp_string_writer(buffer));
    ASSERT_EQ((ssize_t)strlen(HEADER_MAP_STR), ret);
    ASSERT_STR_EQ(HEADER_MAP_STR, buffer);

    PASS();
}

TEST find(void) {
    const SmolRTSP_HeaderMap map = HEADER_MAP;

    CharSlice99 content_length;
    const bool content_length_is_found = SmolRTSP_HeaderMap_find(
        &map, SMOLRTSP_HEADER_CONTENT_LENGTH, &content_length);

    ASSERT(content_length_is_found);
    ASSERT(
        CharSlice99_primitive_eq(content_length, CharSlice99_from_str("10")));

    PASS();
}

TEST contains_key(void) {
    const SmolRTSP_HeaderMap map = HEADER_MAP;

    ASSERT(
        SmolRTSP_HeaderMap_contains_key(&map, SMOLRTSP_HEADER_CONTENT_LENGTH));
    ASSERT(!SmolRTSP_HeaderMap_contains_key(&map, SMOLRTSP_HEADER_ALLOW));

    PASS();
}

TEST append(void) {
    SmolRTSP_HeaderMap map = SmolRTSP_HeaderMap_empty(), expected = HEADER_MAP;

    for (size_t i = 0; i < HEADER_MAP.len; i++) {
        SmolRTSP_HeaderMap_append(&map, HEADER_MAP.headers[i]);
    }

    ASSERT(SmolRTSP_HeaderMap_eq(&map, &expected));
    PASS();
}

TEST is_full(void) {
    SmolRTSP_HeaderMap map = SmolRTSP_HeaderMap_empty();

    ASSERT(!SmolRTSP_HeaderMap_is_full(&map));

    map.len = SMOLRTSP_HEADER_MAP_CAPACITY;
    ASSERT(SmolRTSP_HeaderMap_is_full(&map));

    PASS();
}

TEST scanf_header(void) {
    const SmolRTSP_HeaderMap map = HEADER_MAP;

    size_t content_length = 0;
    int ret = smolrtsp_scanf_header(
        &map, SMOLRTSP_HEADER_CONTENT_LENGTH, "%zd", &content_length);
    ASSERT_EQ(1, ret);
    ASSERT_EQ(10, content_length);

    ret = smolrtsp_scanf_header(&map, SMOLRTSP_HEADER_ACCEPT_LANGUAGE, "%*d");
    ASSERT_EQ(0, ret);

    char auth[32] = {0};
    ret = smolrtsp_scanf_header(
        &map, SMOLRTSP_HEADER_WWW_AUTHENTICATE, "%s", auth);
    ASSERT_EQ(-1, ret);

    PASS();
}

SUITE(types_header_map) {
    RUN_TEST(parse_header_map);
    RUN_TEST(serialize_header_map);
    RUN_TEST(find);
    RUN_TEST(contains_key);
    RUN_TEST(append);
    RUN_TEST(is_full);
    RUN_TEST(scanf_header);
}
