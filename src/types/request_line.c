#include <smolrtsp/types/request_line.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <string.h>

ssize_t SmolRTSP_RequestLine_serialize(SmolRTSP_RequestLine self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    ssize_t result = 0, ret = 0;

    ret = VCALL(w, write, self.method);
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, CharSlice99_from_str(" "));
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, self.uri);
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, CharSlice99_from_str(" "));
    CHK_WRITE_ERR(result, ret);
    ret = SmolRTSP_RtspVersion_serialize(self.version, w);
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, SMOLRTSP_CRLF);
    CHK_WRITE_ERR(result, ret);

    return result;
}

SmolRTSP_ParseResult
SmolRTSP_RequestLine_parse(SmolRTSP_RequestLine *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(SmolRTSP_Method_parse(&self->method, input));
    MATCH(SmolRTSP_RequestURI_parse(&self->uri, input));
    MATCH(SmolRTSP_RtspVersion_parse(&self->version, input));
    MATCH(smolrtsp_match_str(input, "\r\n"));

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_RequestLine_eq(SmolRTSP_RequestLine lhs, SmolRTSP_RequestLine rhs) {
    return CharSlice99_primitive_eq(lhs.method, rhs.method) &&
           CharSlice99_primitive_eq(lhs.uri, rhs.uri) &&
           SmolRTSP_RtspVersion_eq(lhs.version, rhs.version);
}
