#include <smolrtsp/response_line.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <string.h>

void SmolRTSP_ResponseLine_serialize(
    SmolRTSP_ResponseLine self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(user_writer);

    SmolRTSP_RtspVersion_serialize(self.version, user_writer, user_cx);
    user_writer(CharSlice99_from_str(" "), user_cx);
    SmolRTSP_StatusCode_serialize(self.code, user_writer, user_cx);
    user_writer(CharSlice99_from_str(" "), user_cx);
    user_writer(self.reason, user_cx);
    user_writer(SMOLRTSP_CRLF, user_cx);
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
