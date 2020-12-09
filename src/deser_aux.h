#ifndef SMOLRTSP_DESER_AUX_H
#define SMOLRTSP_DESER_AUX_H

#include "aux.h"
#include <smolrtsp/deserialization.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MATCH(deserialize_res)                                                                     \
    do {                                                                                           \
        const SmolRTSP_DeserializeResult deserialize_res_var = deserialize_res;                    \
        if (deserialize_res_var == SmolRTSP_DeserializeResultErr ||                                \
            deserialize_res_var == SmolRTSP_DeserializeResultNeedMore) {                           \
            return deserialize_res_var;                                                            \
        }                                                                                          \
    } while (false)

#define EXPECT(size, data, bytes_read, c)                                                          \
    do {                                                                                           \
        if (*(data) != c) {                                                                        \
            return SmolRTSP_DeserializeResultErr;                                                  \
        }                                                                                          \
                                                                                                   \
        (data)++;                                                                                  \
        (size)--;                                                                                  \
        (bytes_read)++;                                                                            \
    } while (false)

typedef const void *SmolRTSP_Cursor;

SmolRTSP_DeserializeResult SmolRTSP_parse(
    size_t max_data_size, size_t data_size, const void *restrict data, const char *restrict fmt,
    uint32_t entities_to_parse, ...);

SmolRTSP_DeserializeResult SmolRTSP_match_whitespaces(
    size_t *restrict size, const char *restrict *restrict str, size_t *restrict bytes_read);

SmolRTSP_DeserializeResult SmolRTSP_match_non_whitespaces(
    size_t *restrict size, const char *restrict *restrict str, size_t *restrict bytes_read);

SmolRTSP_DeserializeResult SmolRTSP_match_crlf(
    size_t *restrict size, const char *restrict *restrict str, size_t *restrict bytes_read);

SmolRTSP_DeserializeResult SmolRTSP_match_header_name(
    size_t *restrict size, const char *restrict *restrict str, size_t *restrict bytes_read);

#endif // SMOLRTSP_DESER_AUX_H
