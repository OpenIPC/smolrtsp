#ifndef SMOLRTSP_PARSING_H
#define SMOLRTSP_PARSING_H

#include <smolrtsp/common.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <slice99.h>

#define MATCH(parse_res)                                                                           \
    do {                                                                                           \
        const SmolRTSP_ParseResult parse_res_var = parse_res;                                      \
        if (parse_res_var == SmolRTSP_ParseResult_Err ||                                           \
            parse_res_var == SmolRTSP_ParseResult_Pending) {                                       \
            return parse_res_var;                                                                  \
        }                                                                                          \
    } while (0)

#define TRY_PARSE(if_state, expr)                                                                  \
    do {                                                                                           \
        if (state->tag == if_state) {                                                              \
            MATCH(expr);                                                                           \
            state->tag++;                                                                          \
        }                                                                                          \
    } while (0)

SmolRTSP_ParseResult
smolrtsp_match_until(CharSlice99 *restrict data, bool (*matcher)(char c, void *cx), void *cx);
SmolRTSP_ParseResult smolrtsp_match_until_str(CharSlice99 *restrict data, const char *restrict str);

SmolRTSP_ParseResult smolrtsp_match_until_crlf(CharSlice99 *restrict data);
SmolRTSP_ParseResult smolrtsp_match_char(CharSlice99 *restrict data, char c);
SmolRTSP_ParseResult smolrtsp_match_str(CharSlice99 *restrict data, const char *restrict str);
SmolRTSP_ParseResult smolrtsp_match_whitespaces(CharSlice99 *restrict data);
SmolRTSP_ParseResult smolrtsp_match_non_whitespaces(CharSlice99 *restrict data);
SmolRTSP_ParseResult smolrtsp_match_numeric(CharSlice99 *restrict data);
SmolRTSP_ParseResult smolrtsp_match_ident(CharSlice99 *restrict data);
SmolRTSP_ParseResult smolrtsp_match_header_name(CharSlice99 *restrict data);

#endif // SMOLRTSP_PARSING_H
