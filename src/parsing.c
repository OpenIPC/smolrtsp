#include "parsing.h"
#include "correctness.h"

#include <ctype.h>
#include <string.h>

SmolRTSP_DeserializeResult
SmolRTSP_match_until(Slice99 *restrict data, SmolRTSP_Matcher matcher, void *cx) {
    precondition(data);
    precondition(matcher);

    while (!Slice99_is_empty(*data)) {
        if (!matcher(*(char *)data->ptr, cx)) {
            return SmolRTSP_DeserializeResultOk;
        }

        *data = Slice99_advance(*data, 1);
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

SmolRTSP_DeserializeResult
SmolRTSP_match_until_str(Slice99 *restrict data, const char *restrict str) {
    precondition(data);
    precondition(str);

    const size_t str_len = strlen(str);

    precondition(str_len > 0);

    if (data->len < str_len) {
        return SmolRTSP_DeserializeResultPending;
    }

    // An NFA (non-determenistic finite automaton) converted to DFA (determenistic) to recognise a
    // substring in O(data->size).
    size_t states[2] = {0};
    while (!Slice99_is_empty(*data)) {
        states[0] = match_str_transition(*(const char *)data->ptr, states[0], str);
        states[1] = match_str_transition(*(const char *)data->ptr, states[1], str);

        *data = Slice99_advance(*data, 1);

        if (is_str_recognised(states[0], str) || is_str_recognised(states[1], str)) {
            return SmolRTSP_DeserializeResultOk;
        }
    }

    return SmolRTSP_DeserializeResultPending;
}

SmolRTSP_DeserializeResult SmolRTSP_match_until_crlf(Slice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until_str(data, "\r\n");
}

SmolRTSP_DeserializeResult SmolRTSP_match_char(Slice99 *restrict data, char c) {
    precondition(data);

    return SmolRTSP_match_until(data, char_matcher, &c);
}

SmolRTSP_DeserializeResult SmolRTSP_match_str(Slice99 *restrict data, const char *restrict str) {
    precondition(data);
    precondition(str);

    const size_t str_len = strlen(str);

    if (data->len < str_len) {
        return SmolRTSP_DeserializeResultPending;
    }

    if (memcmp(data->ptr, str, str_len) == 0) {
        *data = Slice99_advance(*data, str_len);
        return SmolRTSP_DeserializeResultOk;
    }

    return SmolRTSP_DeserializeResultErr;
}

SmolRTSP_DeserializeResult SmolRTSP_match_whitespaces(Slice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until(data, whitespace_matcher, NULL);
}

SmolRTSP_DeserializeResult SmolRTSP_match_non_whitespaces(Slice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until(data, non_whitespace_matcher, NULL);
}

SmolRTSP_DeserializeResult SmolRTSP_match_numeric(Slice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until(data, numeric_matcher, NULL);
}

SmolRTSP_DeserializeResult SmolRTSP_match_ident(Slice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until(data, ident_matcher, NULL);
}

SmolRTSP_DeserializeResult SmolRTSP_match_header_name(Slice99 *restrict data) {
    precondition(data);

    return SmolRTSP_match_until(data, header_name_char_matcher, NULL);
}
