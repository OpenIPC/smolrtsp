#include <smolrtsp/request_line.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <string.h>

void SmolRTSP_RequestLine_serialize(
    SmolRTSP_RequestLine self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(user_writer);

    user_writer(self.method, user_cx);
    user_writer(CharSlice99_from_str(" "), user_cx);
    user_writer(self.uri, user_cx);
    user_writer(CharSlice99_from_str(" "), user_cx);
    SmolRTSP_RtspVersion_serialize(self.version, user_writer, user_cx);
    user_writer(SMOLRTSP_CRLF, user_cx);
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
