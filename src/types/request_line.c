#include <smolrtsp/types/request_line.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <string.h>

void SmolRTSP_RequestLine_serialize(SmolRTSP_RequestLine self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    VCALL(w, write, self.method);
    VCALL(w, write, CharSlice99_from_str(" "));
    VCALL(w, write, self.uri);
    VCALL(w, write, CharSlice99_from_str(" "));
    SmolRTSP_RtspVersion_serialize(self.version, w);
    VCALL(w, write, SMOLRTSP_CRLF);
}

SmolRTSP_ParseResult SmolRTSP_RequestLine_parse(
    SmolRTSP_RequestLine *restrict self, CharSlice99 input,
    SmolRTSP_RequestLineParseState *restrict state) {
    assert(self);
    assert(state);

    const CharSlice99 backup = input;

    TRY_PARSE(SmolRTSP_RequestLineParseState_Method, SmolRTSP_Method_parse(&self->method, input));

    TRY_PARSE(
        SmolRTSP_RequestLineParseState_RequestURI, SmolRTSP_RequestURI_parse(&self->uri, input));

    TRY_PARSE(
        SmolRTSP_RequestLineParseState_RtspVersion,
        SmolRTSP_RtspVersion_parse(&self->version, input));

    TRY_PARSE(SmolRTSP_RequestLineParseState_Crlf, smolrtsp_match_str(input, "\r\n"));

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_RequestLine_eq(SmolRTSP_RequestLine lhs, SmolRTSP_RequestLine rhs) {
    return CharSlice99_primitive_eq(lhs.method, rhs.method) &&
           CharSlice99_primitive_eq(lhs.uri, rhs.uri) &&
           SmolRTSP_RtspVersion_eq(lhs.version, rhs.version);
}
