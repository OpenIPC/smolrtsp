#ifndef SMOLRTSP_PARSING_H
#define SMOLRTSP_PARSING_H

#include <smolrtsp/common.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MATCH(deserialize_res)                                                                     \
    do {                                                                                           \
        const SmolRTSP_DeserializeResult deserialize_res_var = deserialize_res;                    \
        if (deserialize_res_var == SmolRTSP_DeserializeResultErr ||                                \
            deserialize_res_var == SmolRTSP_DeserializeResultPending) {                            \
            return deserialize_res_var;                                                            \
        }                                                                                          \
    } while (false)

#define TRY_PARSE(if_state, expr)                                                                  \
    do {                                                                                           \
        if (state->tag == if_state) {                                                              \
            MATCH(expr);                                                                           \
            state->tag++;                                                                          \
        }                                                                                          \
    } while (0)

SmolRTSP_DeserializeResult
SmolRTSP_match_until(Slice99 *restrict data, bool (*matcher)(char c, void *cx), void *cx);
SmolRTSP_DeserializeResult
SmolRTSP_match_until_str(Slice99 *restrict data, const char *restrict str);

SmolRTSP_DeserializeResult SmolRTSP_match_until_crlf(Slice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_char(Slice99 *restrict data, char c);
SmolRTSP_DeserializeResult SmolRTSP_match_str(Slice99 *restrict data, const char *restrict str);
SmolRTSP_DeserializeResult SmolRTSP_match_whitespaces(Slice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_non_whitespaces(Slice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_numeric(Slice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_ident(Slice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_header_name(Slice99 *restrict data);

#endif // SMOLRTSP_PARSING_H
