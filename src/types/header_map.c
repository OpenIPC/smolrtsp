#include <smolrtsp/types/header_map.h>

#include "../macros.h"
#include "parsing.h"
#include <smolrtsp/util.h>

#include <assert.h>
#include <string.h>

SmolRTSP_HeaderMap SmolRTSP_HeaderMap_empty(void) {
    return (SmolRTSP_HeaderMap){.headers = {0}, .len = 0};
}

bool SmolRTSP_HeaderMap_find(
    SmolRTSP_HeaderMap self, CharSlice99 key, CharSlice99 *restrict value) {
    assert(value);

    for (size_t i = 0; i < self.len; i++) {
        if (CharSlice99_primitive_eq(self.headers[i].key, key)) {
            *value = self.headers[i].value;
            return true;
        }
    }

    return false;
}

bool SmolRTSP_HeaderMap_key_is_present(SmolRTSP_HeaderMap self, CharSlice99 key) {
    CharSlice99 value;
    return SmolRTSP_HeaderMap_find(self, key, &value);
}

ssize_t SmolRTSP_HeaderMap_serialize(SmolRTSP_HeaderMap self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    ssize_t result = 0;

    for (size_t i = 0; i < self.len; i++) {
        CHK_WRITE_ERR(result, SmolRTSP_Header_serialize(self.headers[i], w));
    }

    CHK_WRITE_ERR(result, VCALL(w, write, SMOLRTSP_CRLF));

    return result;
}

SmolRTSP_ParseResult
SmolRTSP_HeaderMap_parse(SmolRTSP_HeaderMap *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    self->len = 0;

    MATCH(smolrtsp_match_until_double_crlf(input));
    input = backup;

    while (true) {
        if (CharSlice99_primitive_starts_with(input, SMOLRTSP_CRLF)) {
            return SmolRTSP_ParseResult_complete((input.ptr - backup.ptr) + SMOLRTSP_CRLF.len);
        }
        if (SmolRTSP_HeaderMap_is_full(*self)) {
            return SmolRTSP_ParseResult_Failure(SmolRTSP_ParseError_HeaderMapOverflow());
        }

        SmolRTSP_Header header;
        MATCH(SmolRTSP_Header_parse(&header, input));
        self->headers[self->len] = header;
        self->len++;
    }
}

bool SmolRTSP_HeaderMap_eq(SmolRTSP_HeaderMap lhs, SmolRTSP_HeaderMap rhs) {
    if (lhs.len != rhs.len) {
        return false;
    }

    const size_t len = lhs.len;

    for (size_t i = 0; i < len; i++) {
        if (!SmolRTSP_Header_eq(lhs.headers[i], rhs.headers[i])) {
            return false;
        }
    }

    return true;
}

bool SmolRTSP_HeaderMap_is_full(SmolRTSP_HeaderMap self) {
    return SMOLRTSP_HEADER_MAP_CAPACITY == self.len;
}
