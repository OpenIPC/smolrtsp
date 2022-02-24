#include <smolrtsp/types/request_line.h>

#include "../macros.h"
#include "parsing.h"
#include <smolrtsp/util.h>

#include <assert.h>
#include <string.h>

ssize_t
SmolRTSP_RequestLine_serialize(SmolRTSP_RequestLine self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    ssize_t result = 0;

    CHK_WRITE_ERR(result, VCALL(w, write, self.method));
    CHK_WRITE_ERR(result, VCALL(w, write, CharSlice99_from_str(" ")));
    CHK_WRITE_ERR(result, VCALL(w, write, self.uri));
    CHK_WRITE_ERR(result, VCALL(w, write, CharSlice99_from_str(" ")));
    CHK_WRITE_ERR(result, SmolRTSP_RtspVersion_serialize(self.version, w));
    CHK_WRITE_ERR(result, VCALL(w, write, SMOLRTSP_CRLF));

    return result;
}

SmolRTSP_ParseResult SmolRTSP_RequestLine_parse(
    SmolRTSP_RequestLine *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(SmolRTSP_Method_parse(&self->method, input));
    MATCH(SmolRTSP_RequestUri_parse(&self->uri, input));
    MATCH(SmolRTSP_RtspVersion_parse(&self->version, input));
    MATCH(smolrtsp_match_str(input, "\r\n"));

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_RequestLine_eq(
    SmolRTSP_RequestLine lhs, SmolRTSP_RequestLine rhs) {
    return CharSlice99_primitive_eq(lhs.method, rhs.method) &&
           CharSlice99_primitive_eq(lhs.uri, rhs.uri) &&
           SmolRTSP_RtspVersion_eq(lhs.version, rhs.version);
}
