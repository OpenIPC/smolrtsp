#ifndef SMOLRTSP_PARSING_AUX_H
#define SMOLRTSP_PARSING_AUX_H

#include <smolrtsp/deserialization.h>

#include <stdint.h>

typedef const void *SmolRTSP_Cursor;

SmolRTSP_DeserializeResult SmolRTSP_parse(
    size_t max_data_size, size_t data_size, const void *restrict data, const char *restrict fmt,
    uint_least8_t entities_to_parse, ...);

#endif // SMOLRTSP_PARSING_AUX_H
