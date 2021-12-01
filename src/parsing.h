#ifndef SMOLRTSP_PARSING_H
#define SMOLRTSP_PARSING_H

#include <smolrtsp/common.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <slice99.h>

#define MATCH(deserialize_res)                                                                     \
    do {                                                                                           \
        const SmolRTSP_DeserializeResult deserialize_res_var = deserialize_res;                    \
        if (deserialize_res_var == SmolRTSP_DeserializeResult_Err ||                               \
            deserialize_res_var == SmolRTSP_DeserializeResult_Pending) {                           \
            return deserialize_res_var;                                                            \
        }                                                                                          \
    } while (0)

#define TRY_PARSE(if_state, expr)                                                                  \
    do {                                                                                           \
        if (state->tag == if_state) {                                                              \
            MATCH(expr);                                                                           \
            state->tag++;                                                                          \
        }                                                                                          \
    } while (0)

SmolRTSP_DeserializeResult
SmolRTSP_match_until(CharSlice99 *restrict data, bool (*matcher)(char c, void *cx), void *cx);
SmolRTSP_DeserializeResult
SmolRTSP_match_until_str(CharSlice99 *restrict data, const char *restrict str);

SmolRTSP_DeserializeResult SmolRTSP_match_until_crlf(CharSlice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_char(CharSlice99 *restrict data, char c);
SmolRTSP_DeserializeResult SmolRTSP_match_str(CharSlice99 *restrict data, const char *restrict str);
SmolRTSP_DeserializeResult SmolRTSP_match_whitespaces(CharSlice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_non_whitespaces(CharSlice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_numeric(CharSlice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_ident(CharSlice99 *restrict data);
SmolRTSP_DeserializeResult SmolRTSP_match_header_name(CharSlice99 *restrict data);

#endif // SMOLRTSP_PARSING_H
