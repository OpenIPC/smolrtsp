#include "match.h"
#include "aux.h"
#include <smolrtsp/crlf.h>

#include <assert.h>
#include <ctype.h>
#include <string.h>

SmolRTSP_DeserializeResult SmolRTSP_match_until(
    SmolRTSP_Slice *restrict data, size_t *restrict bytes_read, SmolRTSP_Matcher matcher,
    void *cx) {
    assert(data);
    assert(bytes_read);
    assert(matcher);
    assert(!SmolRTSP_Slice_is_null(*data));

    while (!SmolRTSP_Slice_is_empty(*data)) {
        if (!matcher(*(const char *)data->ptr, cx)) {
            return SmolRTSP_DeserializeResultOk;
        }

        *data = SmolRTSP_Slice_advance(*data, 1);
        (*bytes_read)++;
    }

    return SmolRTSP_DeserializeResultOk;
}

static bool whitespace_matcher(char c, void *cx) {
    (void)cx;
    return c == ' ';
}

static bool non_whitespace_matcher(char c, void *cx) {
    (void)cx;
    return !whitespace_matcher(c, NULL);
}

static bool numeric_matcher(char c, void *cx) {
    (void)cx;
    return isdigit(c);
}

static bool ident_matcher(char c, void *cx) {
    (void)cx;
    return isalnum(c) || c == '_';
}

static bool header_name_char_matcher(char c, void *cx) {
    (void)cx;
    return isalpha(c) || c == '-';
}

static bool char_matcher(char c, void *cx) {
    const char expected = *(const char *)cx;
    return c == expected;
}

static size_t match_str_transition(char c, const size_t state, const char *str) {
    if (c == str[state]) {
        return state + 1;
    }

    return 0;
}

static bool is_str_recognised(size_t state, const char *str) {
    return str[state] == '\0';
}

SmolRTSP_DeserializeResult SmolRTSP_match_until_str(
    SmolRTSP_Slice *restrict data, size_t *restrict bytes_read, const char *restrict str) {
    assert(data);
    assert(bytes_read);
    assert(str);
    assert(!SmolRTSP_Slice_is_null(*data));

    const size_t str_len = strlen(str);

    assert(str_len > 0);

    if (data->size < strlen(str)) {
        return SmolRTSP_DeserializeResultNeedMore;
    }

    // An NFA (non-determenistic finite automaton) converted to DFA (determenistic) to recognise a
    // substring in O(data->size).
    size_t states[2] = {0};
    while (!SmolRTSP_Slice_is_empty(*data)) {
        states[0] = match_str_transition(*(const char *)data->ptr, states[0], str);
        states[1] = match_str_transition(*(const char *)data->ptr, states[1], str);
        *data = SmolRTSP_Slice_advance(*data, 1);
        (*bytes_read)++;

        if (is_str_recognised(states[0], str) || is_str_recognised(states[1], str)) {
            return SmolRTSP_DeserializeResultOk;
        }
    }

    return SmolRTSP_DeserializeResultNeedMore;
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

    return SmolRTSP_match_until(data, bytes_read, char_matcher, &c);
}

SmolRTSP_DeserializeResult SmolRTSP_match_str(
    SmolRTSP_Slice *restrict data, size_t *restrict bytes_read, const char *restrict str) {
    assert(data);
    assert(bytes_read);
    assert(str);

    const size_t str_len = strlen(str);

    if (data->size < str_len) {
        return SmolRTSP_DeserializeResultNeedMore;
    }

    if (memcmp(data->ptr, str, str_len) == 0) {
        *data = SmolRTSP_Slice_advance(*data, str_len);
        *bytes_read += str_len;
        return SmolRTSP_DeserializeResultOk;
    }

    return SmolRTSP_DeserializeResultErr;
}

SmolRTSP_DeserializeResult
SmolRTSP_match_whitespaces(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    void *cx = NULL;
    return SmolRTSP_match_until(data, bytes_read, whitespace_matcher, cx);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_non_whitespaces(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    void *cx = NULL;
    return SmolRTSP_match_until(data, bytes_read, non_whitespace_matcher, cx);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_numeric(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    void *cx = NULL;
    return SmolRTSP_match_until(data, bytes_read, numeric_matcher, cx);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_ident(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    void *cx = NULL;
    return SmolRTSP_match_until(data, bytes_read, ident_matcher, cx);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_header_name(SmolRTSP_Slice *restrict data, size_t *restrict bytes_read) {
    assert(data);
    assert(bytes_read);

    void *cx = NULL;
    return SmolRTSP_match_until(data, bytes_read, header_name_char_matcher, cx);
}