#include <smolrtsp/types/header_map.h>

#include "../macros.h"
#include "parsing.h"
#include <smolrtsp/util.h>

#include <assert.h>
#include <string.h>

#include <alloca.h>

SmolRTSP_HeaderMap SmolRTSP_HeaderMap_empty(void) {
    SmolRTSP_HeaderMap self;
    memset(self.headers, '\0', sizeof self.headers);
    self.len = 0;
    return self;
}

bool SmolRTSP_HeaderMap_find(
    const SmolRTSP_HeaderMap *restrict self, CharSlice99 key,
    CharSlice99 *restrict value) {
    assert(self);

    for (size_t i = 0; i < self->len; i++) {
        if (CharSlice99_primitive_eq(self->headers[i].key, key)) {
            if (value != NULL) {
                *value = self->headers[i].value;
            }
            return true;
        }
    }

    return false;
}

bool SmolRTSP_HeaderMap_contains_key(
    const SmolRTSP_HeaderMap *restrict self, CharSlice99 key) {
    assert(self);
    return SmolRTSP_HeaderMap_find(self, key, NULL);
}

void SmolRTSP_HeaderMap_append(
    SmolRTSP_HeaderMap *restrict self, SmolRTSP_Header h) {
    assert(self);
    assert(!SmolRTSP_HeaderMap_is_full(self));

    self->headers[self->len] = h;
    self->len++;
}

ssize_t SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict self, SmolRTSP_Writer w) {
    assert(self);
    assert(w.self && w.vptr);

    ssize_t result = 0;

    for (size_t i = 0; i < self->len; i++) {
        CHK_WRITE_ERR(result, SmolRTSP_Header_serialize(&self->headers[i], w));
    }

    CHK_WRITE_ERR(result, VCALL(w, write, SMOLRTSP_CRLF));

    return result;
}

SmolRTSP_ParseResult
SmolRTSP_HeaderMap_parse(SmolRTSP_HeaderMap *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    self->len = 0;

    while (true) {
        if (CharSlice99_primitive_ends_with(
                input, CharSlice99_from_str("\r"))) {
            return SmolRTSP_ParseResult_partial();
        }
        if (CharSlice99_primitive_starts_with(input, SMOLRTSP_CRLF)) {
            return SmolRTSP_ParseResult_complete(
                (input.ptr - backup.ptr) + SMOLRTSP_CRLF.len);
        }
        if (SmolRTSP_HeaderMap_is_full(self)) {
            return SmolRTSP_ParseResult_Failure(
                SmolRTSP_ParseError_HeaderMapOverflow());
        }

        SmolRTSP_Header header = {0};
        MATCH(SmolRTSP_Header_parse(&header, input));
        SmolRTSP_HeaderMap_append(self, header);
    }
}

bool SmolRTSP_HeaderMap_eq(
    const SmolRTSP_HeaderMap *restrict lhs,
    const SmolRTSP_HeaderMap *restrict rhs) {
    assert(lhs);
    assert(rhs);

    if (lhs->len != rhs->len) {
        return false;
    }

    const size_t len = lhs->len;

    for (size_t i = 0; i < len; i++) {
        if (!SmolRTSP_Header_eq(&lhs->headers[i], &rhs->headers[i])) {
            return false;
        }
    }

    return true;
}

bool SmolRTSP_HeaderMap_is_full(const SmolRTSP_HeaderMap *restrict self) {
    assert(self);
    return SMOLRTSP_HEADER_MAP_CAPACITY == self->len;
}

int smolrtsp_scanf_header(
    const SmolRTSP_HeaderMap *restrict headers, CharSlice99 key,
    const char *restrict fmt, ...) {
    assert(headers);
    assert(fmt);

    CharSlice99 val;
    const bool val_found = SmolRTSP_HeaderMap_find(headers, key, &val);
    if (!val_found) {
        return -1;
    }

    va_list ap;
    va_start(ap, fmt);
    const int ret = vsscanf(CharSlice99_alloca_c_str(val), fmt, ap);
    va_end(ap);

    return ret;
}
