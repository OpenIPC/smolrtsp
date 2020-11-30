#include "parsing_aux.h"

#include <stdarg.h>
#include <stdio.h>

SmolRTSP_DeserializeResult SmolRTSP_parse(
    size_t max_data_size, size_t data_size, const void *restrict data, const char *restrict fmt,
    uint32_t entities_to_parse, ...) {
    va_list args;
    va_start(args, entities_to_parse);

    if (vsscanf(data, fmt, args) != entities_to_parse) {
        if (data_size > max_data_size) {
            return SmolRTSP_DeserializeResultErr;
        }

        return SmolRTSP_DeserializeResultNeedMore;
    }

    va_end(args);
    return SmolRTSP_DeserializeResultOk;
}
