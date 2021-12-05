#include <smolrtsp/response_line.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <string.h>

void SmolRTSP_ResponseLine_serialize(SmolRTSP_ResponseLine self, SmolRTSP_Writer w, void *w_ctx) {
    assert(w);

    SmolRTSP_RtspVersion_serialize(self.version, w, w_ctx);
    w(CharSlice99_from_str(" "), w_ctx);
    SmolRTSP_StatusCode_serialize(self.code, w, w_ctx);
    w(CharSlice99_from_str(" "), w_ctx);
    w(self.reason, w_ctx);
    w(SMOLRTSP_CRLF, w_ctx);
}

SmolRTSP_ParseResult SmolRTSP_ResponseLine_parse(
    SmolRTSP_ResponseLine *restrict self, CharSlice99 input,
    SmolRTSP_ResponseLineParseState *restrict state) {
    assert(self);
    assert(state);

    const CharSlice99 backup = input;

    TRY_PARSE(
        SmolRTSP_ResponseLineParseState_RtspVersion,
        SmolRTSP_RtspVersion_parse(&self->version, input));

    TRY_PARSE(
        SmolRTSP_ResponseLineParseState_StatusCode, SmolRTSP_StatusCode_parse(&self->code, input));

    TRY_PARSE(
        SmolRTSP_ResponseLineParseState_ReasonPhrase,
        SmolRTSP_ReasonPhrase_parse(&self->reason, input));

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_ResponseLine_eq(SmolRTSP_ResponseLine lhs, SmolRTSP_ResponseLine rhs) {
    return SmolRTSP_RtspVersion_eq(lhs.version, rhs.version) && lhs.code == rhs.code &&
           CharSlice99_primitive_eq(lhs.reason, rhs.reason);
}
