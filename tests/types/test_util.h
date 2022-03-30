#pragma once

#include <smolrtsp/types/error.h>

#include <stddef.h>

#include <greatest.h>
#include <slice99.h>

#define TEST_PARSE(input, expected) CHECK_CALL(test_parse(input, expected))

#ifndef TEST_PARSE_INIT_TYPE
#define TEST_PARSE_INIT_TYPE(_result)
#endif

#ifndef TEST_PARSE_DEINIT_TYPE
#define TEST_PARSE_DEINIT_TYPE(_result)
#endif

#define DEF_TEST_PARSE(T)                                                      \
    static enum greatest_test_res test_parse(const char *str, T expected) {    \
        T result;                                                              \
        TEST_PARSE_INIT_TYPE(result);                                          \
        const CharSlice99 input = CharSlice99_from_str((char *)str);           \
        SmolRTSP_ParseResult ret;                                              \
                                                                               \
        for (size_t i = 1; i < input.len; i++) {                               \
            ret = T##_parse(&result, CharSlice99_sub(input, 0, i));            \
            ASSERT(SmolRTSP_ParseResult_is_partial(ret));                      \
        }                                                                      \
                                                                               \
        ret = T##_parse(&result, input);                                       \
        match(ret) {                                                           \
            of(SmolRTSP_ParseResult_Success, status) {                         \
                ASSERT(SmolRTSP_ParseStatus_is_complete(*status));             \
                /* ASSERT_EQ(input.len, status->offset); */                    \
                ASSERT(T##_eq(&result, &expected));                            \
            }                                                                  \
            of(SmolRTSP_ParseResult_Failure, error) {                          \
                const int err_bytes = SmolRTSP_ParseError_print(               \
                    *error, smolrtsp_file_writer(stderr));                     \
                ASSERT(err_bytes >= 0);                                        \
                FAILm("Parsing failed");                                       \
            }                                                                  \
        }                                                                      \
                                                                               \
        TEST_PARSE_DEINIT_TYPE(result);                                        \
        PASS();                                                                \
    }
