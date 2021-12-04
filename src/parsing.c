#include "parsing.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>

SmolRTSP_ParseResult
smolrtsp_match_until(CharSlice99 input, bool (*matcher)(char c, void *ctx), void *ctx) {
    assert(matcher);

    size_t offset = 0;

    while (!CharSlice99_is_empty(input)) {
        if (!matcher(*(char *)input.ptr, ctx)) {
            return SmolRTSP_ParseResult_complete(offset);
        }

        input = CharSlice99_advance(input, 1);
        offset++;
    }

    return SmolRTSP_ParseResult_partial(offset);
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

SmolRTSP_ParseResult smolrtsp_match_until_str(CharSlice99 input, const char *restrict str) {
    assert(str);

    const size_t str_len = strlen(str);
    assert(str_len > 0);

    size_t offset = 0;

    if (input.len < str_len) {
        return SmolRTSP_ParseResult_partial(offset);
    }

    // An NFA (non-determenistic finite automaton) converted to DFA (determenistic) to recognise a
    // substring in O(input.size).
    size_t states[2] = {0};
    while (!CharSlice99_is_empty(input)) {

#define TRANSITION(c, state, str) ((c) == (str)[(state)] ? (state) + 1 : 0)

        states[0] = TRANSITION(*input.ptr, states[0], str);
        states[1] = TRANSITION(*input.ptr, states[1], str);

#undef TRANSITION

        input = CharSlice99_advance(input, 1);
        offset++;

#define IS_RECOGNIZED(state, str) ('\0' == (str)[(state)])

        if (IS_RECOGNIZED(states[0], str) || IS_RECOGNIZED(states[1], str)) {
            return SmolRTSP_ParseResult_complete(offset);
        }

#undef IS_RECOGNIZED
    }

    return SmolRTSP_ParseResult_partial(offset);
}

SmolRTSP_ParseResult smolrtsp_match_until_crlf(CharSlice99 input) {
    return smolrtsp_match_until_str(input, "\r\n");
}

SmolRTSP_ParseResult smolrtsp_match_char(CharSlice99 input, char c) {
    return smolrtsp_match_until(input, char_matcher, &c);
}

SmolRTSP_ParseResult smolrtsp_match_str(CharSlice99 input, const char *restrict str) {
    assert(str);

    const size_t str_len = strlen(str);

    size_t offset = 0;

    const size_t min_len = input.len < str_len ? input.len : str_len;

    const bool are_coinciding = memcmp(input.ptr, str, min_len) == 0;
    if (!are_coinciding) {
        CharSlice99 expected = CharSlice99_from_str((char *)str),
                    actual = CharSlice99_sub(input, 0, min_len);
        return SmolRTSP_ParseResult_Failure(SmolRTSP_ParseError_StrMismatch(expected, actual));
    }

    if (input.len < str_len) {
        return SmolRTSP_ParseResult_partial(offset);
    }

    input = CharSlice99_advance(input, str_len);
    offset += str_len;
    return SmolRTSP_ParseResult_complete(offset);
}

SmolRTSP_ParseResult smolrtsp_match_whitespaces(CharSlice99 input) {
    return smolrtsp_match_until(input, whitespace_matcher, NULL);
}

SmolRTSP_ParseResult smolrtsp_match_non_whitespaces(CharSlice99 input) {
    return smolrtsp_match_until(input, non_whitespace_matcher, NULL);
}

SmolRTSP_ParseResult smolrtsp_match_numeric(CharSlice99 input) {
    const SmolRTSP_ParseResult res = smolrtsp_match_until(input, numeric_matcher, NULL);

    ifLet(res, SmolRTSP_ParseResult_Success, status) {
        const bool not_numeric = 0 == status->offset;
        if (status->is_complete && not_numeric) {
            return SmolRTSP_ParseResult_Failure(
                SmolRTSP_ParseError_TypeMismatch(SmolRTSP_ParseType_Int, input));
        }
    }

    return res;
}

SmolRTSP_ParseResult smolrtsp_match_ident(CharSlice99 input) {
    const SmolRTSP_ParseResult res = smolrtsp_match_until(input, ident_matcher, NULL);

    ifLet(res, SmolRTSP_ParseResult_Success, status) {
        const bool not_ident = 0 == status->offset;
        if (status->is_complete && not_ident) {
            return SmolRTSP_ParseResult_Failure(
                SmolRTSP_ParseError_TypeMismatch(SmolRTSP_ParseType_Ident, input));
        }
    }

    return res;
}

SmolRTSP_ParseResult smolrtsp_match_header_name(CharSlice99 input) {
    const SmolRTSP_ParseResult res = smolrtsp_match_until(input, header_name_char_matcher, NULL);

    ifLet(res, SmolRTSP_ParseResult_Success, status) {
        const bool not_header_name = 0 == status->offset;
        if (status->is_complete && not_header_name) {
            return SmolRTSP_ParseResult_Failure(
                SmolRTSP_ParseError_TypeMismatch(SmolRTSP_ParseType_HeaderName, input));
        }
    }

    return res;
}
