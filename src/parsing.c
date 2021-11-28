#include "parsing.h"

#include "correctness.h"

#include <ctype.h>
#include <math.h>
#include <string.h>

SmolRTSP_DeserializeResult
SmolRTSP_match_until(CharSlice99 *restrict data, bool (*matcher)(char c, void *cx), void *cx) {
    precondition(data);
    precondition(matcher);

    while (!CharSlice99_is_empty(*data)) {
        if (!matcher(*(char *)data->ptr, cx)) {
            return SmolRTSP_DeserializeResult_Ok;
        }

        *data = CharSlice99_advance(*data, 1);
    }

    return SmolRTSP_DeserializeResult_Pending;
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

SmolRTSP_DeserializeResult
SmolRTSP_match_until_str(CharSlice99 *restrict data, const char *restrict str) {
    precondition(data);
    precondition(str);

    const size_t str_len = strlen(str);

    precondition(str_len > 0);

    if (data->len < str_len) {
        return SmolRTSP_DeserializeResult_Pending;
    }

    // An NFA (non-determenistic finite automaton) converted to DFA (determenistic) to recognise a
    // substring in O(data->size).
    size_t states[2] = {0};
    while (!CharSlice99_is_empty(*data)) {
        states[0] = match_str_transition(*(const char *)data->ptr, states[0], str);
        states[1] = match_str_transition(*(const char *)data->ptr, states[1], str);

        *data = CharSlice99_advance(*data, 1);

        if (is_str_recognised(states[0], str) || is_str_recognised(states[1], str)) {
            return SmolRTSP_DeserializeResult_Ok;
        }
    }

    return SmolRTSP_DeserializeResult_Pending;
}

SmolRTSP_DeserializeResult SmolRTSP_match_until_crlf(CharSlice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until_str(data, "\r\n");
}

SmolRTSP_DeserializeResult SmolRTSP_match_char(CharSlice99 *restrict data, char c) {
    precondition(data);

    return SmolRTSP_match_until(data, char_matcher, &c);
}

SmolRTSP_DeserializeResult
SmolRTSP_match_str(CharSlice99 *restrict data, const char *restrict str) {
    precondition(data);
    precondition(str);

    const size_t str_len = strlen(str);

    const bool are_coinciding = memcmp(data->ptr, str, fmin(data->len, str_len)) == 0;
    if (!are_coinciding) {
        return SmolRTSP_DeserializeResult_Err;
    }

    if (data->len < str_len) {
        return SmolRTSP_DeserializeResult_Pending;
    }

    *data = CharSlice99_advance(*data, str_len);
    return SmolRTSP_DeserializeResult_Ok;
}

SmolRTSP_DeserializeResult SmolRTSP_match_whitespaces(CharSlice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until(data, whitespace_matcher, NULL);
}

SmolRTSP_DeserializeResult SmolRTSP_match_non_whitespaces(CharSlice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until(data, non_whitespace_matcher, NULL);
}

SmolRTSP_DeserializeResult SmolRTSP_match_numeric(CharSlice99 *restrict data) {
    precondition(data);

    const CharSlice99 backup = *data;

    const SmolRTSP_DeserializeResult res = SmolRTSP_match_until(data, numeric_matcher, NULL);
    const bool not_numeric = backup.len == data->len && backup.ptr == data->ptr && data->len > 0;
    if (not_numeric) {
        return SmolRTSP_DeserializeResult_Err;
    }

    return res;
}

SmolRTSP_DeserializeResult SmolRTSP_match_ident(CharSlice99 *restrict data) {
    precondition(data);

    const CharSlice99 backup = *data;

    const SmolRTSP_DeserializeResult res = SmolRTSP_match_until(data, ident_matcher, NULL);
    const bool not_ident = backup.len == data->len && backup.ptr == data->ptr && data->len > 0;
    if (not_ident) {
        return SmolRTSP_DeserializeResult_Err;
    }

    return res;
}

SmolRTSP_DeserializeResult SmolRTSP_match_header_name(CharSlice99 *restrict data) {
    precondition(data);

    const CharSlice99 backup = *data;

    const SmolRTSP_DeserializeResult res =
        SmolRTSP_match_until(data, header_name_char_matcher, NULL);
    const bool not_header_name =
        backup.len == data->len && backup.ptr == data->ptr && data->len > 0;
    if (not_header_name) {
        return SmolRTSP_DeserializeResult_Err;
    }

    return res;
}
