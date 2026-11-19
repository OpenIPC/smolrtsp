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
    // The length limit holds while the value is still arriving, not only once
    // its CRLF is found: a value that never ends would otherwise be Partial
    // for as long as the peer keeps sending, and buffered all the while.
    if (SmolRTSP_ParseResult_is_partial(smolrtsp_match_until_crlf(input)) &&
        input.len > SMOLRTSP_MAX_HEADER_VALUE + strlen("\r\n")) {
        return SmolRTSP_ParseResult_Failure(
            SmolRTSP_ParseError_HeaderValueTooLong(input));
    }
    MATCH(smolrtsp_match_until_crlf(input));
    header.value =
        CharSlice99_from_ptrdiff(header.value.ptr, input.ptr - strlen("\r\n"));

    if (header.value.len > SMOLRTSP_MAX_HEADER_VALUE) {
        return SmolRTSP_ParseResult_Failure(
            SmolRTSP_ParseError_HeaderValueTooLong(header.value));
    }

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
