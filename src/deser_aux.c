#include "deser_aux.h"
#include "aux.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

SmolRTSP_DeserializeResult SmolRTSP_parse(
    size_t max_data_size, size_t data_size, const void *restrict data, const char *restrict fmt,
    uint32_t entities_to_parse, ...) {
    assert(data);
    assert(fmt);

    SmolRTSP_DeserializeResult res;
    va_list args;
    va_start(args, entities_to_parse);

    if (vsscanf(data, fmt, args) == entities_to_parse) {
        res = SmolRTSP_DeserializeResultOk;
    } else if (data_size > max_data_size) {
        res = SmolRTSP_DeserializeResultErr;
    } else {
        res = SmolRTSP_DeserializeResultNeedMore;
    }

cleanup:
    va_end(args);
    return res;
}

SmolRTSP_DeserializeResult SmolRTSP_match_whitespaces(
    size_t *restrict size, const char *restrict *restrict str, size_t *restrict bytes_read) {
    assert(size);
    assert(str);
    assert(bytes_read);

    for (size_t i = 0; i < *size; i++) {
        if ((*str)[i] != ' ') {
            *bytes_read += i;
            SUB_SLICE(*size, *str, i);
            return SmolRTSP_DeserializeResultOk;
        }
    }

    return SmolRTSP_DeserializeResultNeedMore;
}

SmolRTSP_DeserializeResult SmolRTSP_match_non_whitespaces(
    size_t *restrict size, const char *restrict *restrict str, size_t *restrict bytes_read) {
    assert(size);
    assert(str);
    assert(bytes_read);

    for (size_t i = 0; i < *size; i++) {
        if ((*str)[i] == ' ') {
            *bytes_read += i;
            SUB_SLICE(*size, *str, i);
            return SmolRTSP_DeserializeResultOk;
        }
    }

    return SmolRTSP_DeserializeResultNeedMore;
}

SmolRTSP_DeserializeResult SmolRTSP_match_crlf(
    size_t *restrict size, const char *restrict *restrict str, size_t *restrict bytes_read) {
    assert(size);
    assert(str);
    assert(bytes_read);

    for (size_t i = 0; i < *size - 1; i++) {
        if ((*str)[i] == '\r' && (*str)[i + 1] == '\n') {
            *bytes_read += i + 2;
            SUB_SLICE(*size, *str, i + 2);
            return SmolRTSP_DeserializeResultOk;
        }
    }

    return SmolRTSP_DeserializeResultNeedMore;
}

SmolRTSP_DeserializeResult SmolRTSP_match_header_name(
    size_t *restrict size, const char *restrict *restrict str, size_t *restrict bytes_read) {
    assert(size);
    assert(str);
    assert(bytes_read);

    for (size_t i = 0; i < *size; i++) {
        if (!isalpha((*str)[i]) && (*str)[i] != '-') {
            *bytes_read += i;
            SUB_SLICE(*size, *str, i);
            return SmolRTSP_DeserializeResultOk;
        }
    }

    return SmolRTSP_DeserializeResultNeedMore;
}
