#include <smolrtsp/types/header.h>

#include "parsing.h"
#include <smolrtsp/util.h>

#include <assert.h>
#include <string.h>

ssize_t SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self, SmolRTSP_Writer w) {
    assert(self);
    assert(w.self && w.vptr);

    return SMOLRTSP_WRITE_SLICES(
        w, {
               self->key,
               CharSlice99_from_str(": "),
               self->value,
               SMOLRTSP_CRLF,
           });
}

SmolRTSP_ParseResult
SmolRTSP_Header_parse(SmolRTSP_Header *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    SmolRTSP_Header header;

    MATCH(smolrtsp_match_whitespaces(input));
    header.key = input;
    MATCH(smolrtsp_match_header_name(input));
    header.key = CharSlice99_from_ptrdiff(header.key.ptr, input.ptr);

    MATCH(smolrtsp_match_whitespaces(input));
    MATCH(smolrtsp_match_char(input, ':'));
    MATCH(smolrtsp_match_whitespaces(input));

    header.value = input;
    MATCH(smolrtsp_match_until_crlf(input));
    header.value =
        CharSlice99_from_ptrdiff(header.value.ptr, input.ptr - strlen("\r\n"));

    *self = header;

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_Header_eq(
    const SmolRTSP_Header *restrict lhs, const SmolRTSP_Header *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return CharSlice99_primitive_eq(lhs->key, rhs->key) &&
           CharSlice99_primitive_eq(lhs->value, rhs->value);
}
