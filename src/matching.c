#include "matching.h"
#include "aux.h"
#include <smolrtsp/crlf.h>

#include <assert.h>
#include <ctype.h>
#include <string.h>

SmolRTSP_DeserializeResult SmolRTSP_match_until(
    SmolRTSP_Slice *restrict data, size_t *restrict bytes_read, SmolRTSP_Matcher matcher, void *cx,
    size_t matched_str_len) {
    assert(data);
    assert(bytes_read);
    assert(matcher);
    assert(matched_str_len >= 1);

    if (data->size < matched_str_len) {
        return SmolRTSP_DeserializeResultNeedMore;
    }

    size_t i = 0;
    for (; i < data->size - matched_str_len + 1; i++) {
        if (!matcher(data->ptr + i, cx)) {
            const size_t bytes_read_temp = matched_str_len == 1 ? i : (i + matched_str_len + 1);
            SUB_SLICE(data->size, data->ptr, bytes_read_temp);
            //            *data = SmolRTSP_Slice_advance(*data, bytes_read_temp);
            *bytes_read += bytes_read_temp;
            return SmolRTSP_DeserializeResultOk;
        }
    }

    const size_t bytes_read_temp = matched_str_len == 1 ? i : (i + matched_str_len + 1);
    SUB_SLICE(data->size, data->ptr, bytes_read_temp);
    *bytes_read += bytes_read_temp;
    return SmolRTSP_DeserializeResultOk;
}

static bool until_str_matcher(const char *sub_str, void *cx) {
    assert(sub_str);
    assert(cx);
    const char *expected_str = cx;
    return memcmp(sub_str, expected_str, strlen(expected_str)) != 0;
}

static bool whitespace_matcher(const char *sub_str, void *cx) {
    (void)cx;
    assert(sub_str);
    return *sub_str == ' ';
}

static bool non_whitespace_matcher(const char *sub_str, void *cx) {
    (void)cx;
    assert(sub_str);
    return !whitespace_matcher(sub_str, NULL);
}

static bool numeric_matcher(const char *sub_str, void *cx) {
    (void)cx;
    assert(sub_str);
    return isdigit(*sub_str);
}

static bool ident_matcher(const char *sub_str, void *cx) {
    (void)cx;
    assert(sub_str);
    return isalnum(*sub_str) || *sub_str == '_';
}

static bool header_name_char_matcher(const char *sub_str, void *cx) {
    (void)cx;
    assert(sub_str);
    return isalpha(*sub_str) || *sub_str == '-';
}

SmolRTSP_DeserializeResult SmolRTSP_match_until_str(
    SmolRTSP_Slice *restrict data, size_t *restrict bytes_read, const char *restrict expected_str) {
    assert(data);
    assert(bytes_read);
    assert(expected_str);

    const size_t expected_str_len = strlen(expected_str);

    return SmolRTSP_match_until(
        data, bytes_read, until_str_matcher, (void *)expected_str, expected_str_len);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_until_crlf(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    return SmolRTSP_match_until_str(data, bytes_read, SMOLRTSP_CRLF);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_char(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read, char c) {
    assert(data);
    assert(bytes_read);

    return SmolRTSP_match_str(data, bytes_read, (const char[]){c, '\0'});
}

SmolRTSP_DeserializeResult SmolRTSP_match_str(
    SmolRTSP_Slice *restrict data, size_t *restrict bytes_read, const char *restrict expected_str) {
    assert(data);
    assert(bytes_read);
    assert(expected_str);

    const size_t expected_str_len = strlen(expected_str);

    if (data->size < expected_str_len) {
        return SmolRTSP_DeserializeResultNeedMore;
    }

    if (memcmp(data->ptr, expected_str, expected_str_len) == 0) {
        *data = SmolRTSP_Slice_advance(*data, expected_str_len);
        *bytes_read += expected_str_len;
        return SmolRTSP_DeserializeResultOk;
    }

    return SmolRTSP_DeserializeResultErr;
}

SmolRTSP_DeserializeResult
SmolRTSP_match_whitespaces(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    return SmolRTSP_match_until(data, bytes_read, whitespace_matcher, NULL, 1);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_non_whitespaces(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    return SmolRTSP_match_until(data, bytes_read, non_whitespace_matcher, NULL, 1);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_numeric(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    return SmolRTSP_match_until(data, bytes_read, numeric_matcher, NULL, 1);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_ident(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    return SmolRTSP_match_until(data, bytes_read, ident_matcher, NULL, 1);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_header_name(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    return SmolRTSP_match_until(data, bytes_read, header_name_char_matcher, NULL, 1);
}
