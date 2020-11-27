#ifndef NALA_H
#define NALA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define NALA_VERSION "0.177.0"

/**
 * Assert that given characters, numbers, pointers or strings are
 * equal.
 */
#define ASSERT_EQ(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_EQ)

/**
 * Assert that given characters, numbers, pointers or strings are not
 * equal.
 */
#define ASSERT_NE(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_NE)

/**
 * Assert that actual is less than expected.
 */
#define ASSERT_LT(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_LT)

/**
 * Assert that actual is less than or equal to expected.
 */
#define ASSERT_LE(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_LE)

/**
 * Assert that actual is greater than expected.
 */
#define ASSERT_GT(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_GT)

/**
 * Assert that actual is greater than or equal to expected.
 */
#define ASSERT_GE(actual, expected)                                     \
    NALA_ASSERT_FUNC(actual)((actual), (expected), NALA_CHECK_GE)

/**
 * Assert that given value is true.
 */
#define ASSERT_TRUE(actual) nala_assert_true(actual)

/**
 * Assert that given value is false.
 */
#define ASSERT_FALSE(actual) nala_assert_false(actual)

/**
 * Assert that given haystack string contains given needle string.
 */
#define ASSERT_SUBSTRING(haystack, needle)      \
    nala_assert_substring(haystack, needle)

/**
 * Assert that given haystack string does not contain given needle
 * string.
 */
#define ASSERT_NOT_SUBSTRING(haystack, needle)  \
    nala_assert_not_substring(haystack, needle)

/**
 * Assert that given memory regions are equal.
 */
#define ASSERT_MEMORY_EQ(actual, expected, size)        \
    nala_assert_memory(actual, expected, size)

/**
 * Assert that given files are equal.
 */
#define ASSERT_FILE_EQ(actual, expected)        \
    nala_assert_file_eq(actual, expected)

/**
 * Assert that given arrays are equal.
 */
#define ASSERT_ARRAY_EQ(actual, expected, size)                        \
    NALA_ASSERT_ARRAY_FUNC(actual)(actual, expected, sizeof((actual)[0]), size)

/**
 * Assert that given function pointers are equal.
 */
#define ASSERT_FUNCTION_POINTER_EQ(actual, expected)            \
    if (actual != expected) {                                   \
        nala_test_failure(nala_format("%p is not equal to %p",  \
                                      actual,                   \
                                      expected));               \
    }

/**
 * Assert that given function pointers are not equal.
 */
#define ASSERT_FUNCTION_POINTER_NE(actual, expected)            \
    if (actual == expected) {                                   \
        nala_test_failure(nala_format("%p is equal to %p",      \
                                      actual,                   \
                                      expected));               \
    }

/**
 * Assert that given condition is true.
 */
#define ASSERT(cond) nala_assert(cond)

/**
 * Fail current test with given message.
 */
#define FAIL(message) nala_fail(message)

/**
 * A capture output block.
 */
#define CAPTURE_OUTPUT(stdout_name, stderr_name)                        \
    int NALA_UNIQUE(stdout_name);                                       \
    static char *stdout_name = NULL;                                    \
    static char *stderr_name = NULL;                                    \
                                                                        \
    for (NALA_UNIQUE(stdout_name) = 0,                                  \
             nala_capture_output_start(&stdout_name, &stderr_name);     \
         NALA_UNIQUE(stdout_name) < 1;                                  \
         NALA_UNIQUE(stdout_name)++, nala_capture_output_stop())

/**
 * Additional message on error block. May be nested.
 */
#define WITH_MESSAGE(format, ...)                                       \
    int NALA_UNIQUE(i);                                                 \
    for (NALA_UNIQUE(i) = 0, nala_with_message_push(format, ##__VA_ARGS__); \
         NALA_UNIQUE(i) < 1;                                            \
         NALA_UNIQUE(i)++, nala_with_message_pop())

/**
 * A test case.
 */
#define TEST(name)                                      \
    static void name(void);                             \
    static void name ## _before_fork() {}               \
    static struct nala_test_t nala_test_ ## name = {    \
        .name_p = #name,                                \
        .file_p = __FILE__,                             \
        .func = name,                                   \
        .before_fork_func = name ## _before_fork,       \
        .next_p = NULL                                  \
    };                                                  \
    __attribute__ ((constructor))                       \
    static void nala_constructor_ ## name(void)         \
    {                                                   \
        nala_register_test(&nala_test_ ## name);        \
    }                                                   \
    static void name(void)

/**
 * Allocate a memory buffer of given size that is automatically freed
 * after the test. Always returns a valid pointer.
 */
void *nala_alloc(size_t size);

/**
 * Automatically free given buffer after the test. free() is called to
 * free the buffer.
 */
void nala_auto_free(void *buf_p);

/**
 * Performs post-test checks and cleanup, and then exits with status
 * 0. The status parameter is ignored.
 */
void nala_exit(int status);

/*
 * Everything below is for Nala-internal use only!
 */

#define NALA_CHECK_EQ  0
#define NALA_CHECK_NE  1
#define NALA_CHECK_LT  2
#define NALA_CHECK_LE  3
#define NALA_CHECK_GT  4
#define NALA_CHECK_GE  5

#define NALA_TOKENPASTE(x, y) NALA_TOKENPASTE2(x, y)

#define NALA_TOKENPASTE2(x, y) x ## y

#define NALA_UNIQUE(x)  NALA_TOKENPASTE(x, NALA_TOKENPASTE(___, __LINE__))

struct nala_traceback_t {
    void *addresses[32];
    int depth;
};

#define NALA_ASSERT_FUNC(value)                         \
    _Generic((value),                                   \
             char: nala_assert_char,                    \
             signed char: nala_assert_schar,            \
             unsigned char: nala_assert_uchar,          \
             short: nala_assert_short,                  \
             unsigned short: nala_assert_ushort,        \
             int: nala_assert_int,                      \
             unsigned int: nala_assert_uint,            \
             long: nala_assert_long,                    \
             unsigned long: nala_assert_ulong,          \
             long long: nala_assert_llong,              \
             unsigned long long: nala_assert_ullong,    \
             float: nala_assert_float,                  \
             double: nala_assert_double,                \
             long double: nala_assert_ldouble,          \
             char *: nala_assert_string,                \
             const char *: nala_assert_string,          \
             bool: nala_assert_bool,                    \
             default: nala_assert_ptr)

#define NALA_ASSERT_ARRAY_FUNC(value)                                   \
    _Generic((value),                                                   \
             char *: nala_assert_array_char,                            \
             const char *: nala_assert_array_char,                      \
             signed char *: nala_assert_array_schar,                    \
             const signed char *: nala_assert_array_schar,              \
             unsigned char *: nala_assert_array_uchar,                  \
             const unsigned char *: nala_assert_array_uchar,            \
             short *: nala_assert_array_short,                          \
             const short *: nala_assert_array_short,                    \
             unsigned short *: nala_assert_array_ushort,                \
             const unsigned short *: nala_assert_array_ushort,          \
             int *: nala_assert_array_int,                              \
             const int *: nala_assert_array_int,                        \
             unsigned int *: nala_assert_array_uint,                    \
             const unsigned int *: nala_assert_array_uint,              \
             long *: nala_assert_array_long,                            \
             const long *: nala_assert_array_long,                      \
             unsigned long *: nala_assert_array_ulong,                  \
             const unsigned long *: nala_assert_array_ulong,            \
             long long *: nala_assert_array_llong,                      \
             const long long *: nala_assert_array_llong,                \
             unsigned long long *: nala_assert_array_ullong,            \
             const unsigned long long *: nala_assert_array_ullong,      \
             float *: nala_assert_array_float,                          \
             const float *: nala_assert_array_float,                    \
             double *: nala_assert_array_double,                        \
             const double *: nala_assert_array_double,                  \
             long double *: nala_assert_array_ldouble,                  \
             const long double *: nala_assert_array_ldouble,            \
             bool *: nala_assert_array_bool,                            \
             const bool *: nala_assert_array_bool,                      \
             default: nala_assert_array)

struct nala_test_t {
    const char *name_p;
    const char *file_p;
    void (*func)(void);
    void (*before_fork_func)(void);
    struct {
        bool executed;
        int exit_code;
        int signal_number;
        float elapsed_time_ms;
    } run;
    struct nala_test_t *next_p;
};

bool nala_check_string_equal(const char *actual_p, const char *expected_p);

const char *nala_format(const char *format_p, ...);

const char *nala_format_string(const char *prefix_p,
                               const char *actual_p,
                               const char *expected_p);

const char *nala_format_memory(const char *prefix_p,
                               const void *actual_p,
                               const void *expected_p,
                               size_t size);

bool nala_check_memory(const void *left_p, const void *right_p, size_t size);

void nala_capture_output_start(char **stdout_pp, char **stderr_pp);

void nala_capture_output_stop(void);

FILE *nala_get_stdout(void);

/**
 * message_p is freed by this function.
 */
__attribute__ ((noreturn)) void nala_test_failure(const char *message_p);

void nala_register_test(struct nala_test_t *test_p);

int nala_run_tests(void);

void nala_reset_all_mocks(void);

char *nala_mock_traceback_format(void **buffer_pp, int depth);

void nala_assert_char(char actual, char expected, int op);

void nala_assert_schar(signed char actual, signed char expected, int op);

void nala_assert_uchar(unsigned char actual, unsigned char expected, int op);

void nala_assert_short(short actual, short expected, int op);

void nala_assert_ushort(unsigned short actual, unsigned short expected, int op);

void nala_assert_int(int actual, int expected, int op);

void nala_assert_uint(unsigned int actual, unsigned int expected, int op);

void nala_assert_long(long actual, long expected, int op);

void nala_assert_ulong(unsigned long actual, unsigned long expected, int op);

void nala_assert_llong(long long actual, long long expected, int op);

void nala_assert_ullong(unsigned long long actual,
                        unsigned long long expected,
                        int op);

void nala_assert_float(float actual, float expected, int op);

void nala_assert_double(double actual, double expected, int op);

void nala_assert_ldouble(long double actual, long double expected, int op);

void nala_assert_bool(bool actual, bool expected, int op);

void nala_assert_ptr(const void *actual_p, const void *expected_p, int op);

void nala_assert_array_char(const char *actual_p,
                            const char *expected_p,
                            size_t item_size,
                            size_t size);

void nala_assert_array_schar(const signed char *actual_p,
                             const signed char *expected_p,
                             size_t item_size,
                             size_t size);

void nala_assert_array_uchar(const unsigned char *actual_p,
                             const unsigned char *expected_p,
                             size_t item_size,
                             size_t size);

void nala_assert_array_short(const short *actual_p,
                             const short *expected_p,
                             size_t item_size,
                             size_t size);

void nala_assert_array_ushort(const unsigned short *actual_p,
                              const unsigned short *expected_p,
                              size_t item_size,
                              size_t size);

void nala_assert_array_int(const int *actual_p,
                           const int *expected_p,
                           size_t item_size,
                           size_t size);

void nala_assert_array_uint(const unsigned int *actual_p,
                            const unsigned int *expected_p,
                            size_t item_size,
                            size_t size);

void nala_assert_array_long(const long *actual_p,
                            const long *expected_p,
                            size_t item_size,
                            size_t size);

void nala_assert_array_ulong(const unsigned long *actual_p,
                             const unsigned long *expected_p,
                             size_t item_size,
                             size_t size);

void nala_assert_array_llong(const long long *actual_p,
                             const long long *expected_p,
                             size_t item_size,
                             size_t size);

void nala_assert_array_ullong(const unsigned long long *actual_p,
                              const unsigned long long *expected_p,
                              size_t item_size,
                              size_t size);

void nala_assert_array_float(const float *actual_p,
                             const float *expected_p,
                             size_t item_size,
                             size_t size);

void nala_assert_array_double(const double *actual_p,
                              const double *expected_p,
                              size_t item_size,
                              size_t size);

void nala_assert_array_ldouble(const long double *actual_p,
                               const long double *expected_p,
                               size_t item_size,
                               size_t size);

void nala_assert_array_bool(const bool *actual_p,
                            const bool *expected_p,
                            size_t item_size,
                            size_t size);

void nala_assert_array(const void *actual_p,
                       const void *expected_p,
                       size_t item_size,
                       size_t size);

void nala_assert_string(const char *actual_p, const char *expected_p, int op);

void nala_assert_substring(const char *haystack_p, const char *needle_p);

void nala_assert_not_substring(const char *haystack_p, const char *needle_p);

void nala_assert_memory(const void *actual_p, const void *expected_p, size_t size);

void nala_assert_string_or_memory(const void *actual_p,
                                  const void *expected_p,
                                  size_t size);

void nala_assert_file_eq(const char *actual_p, const char *expected_p);

void nala_assert_true(bool actual);

void nala_assert_false(bool actual);

void nala_assert(bool cond);

void nala_fail(const char *message_p);

void nala_with_message_push(const char *format_p, ...);

void nala_with_message_pop(void);

#ifdef NALA_INCLUDE_NALA_MOCKS_H
#    include "nala_mocks.h"
#endif

#endif
