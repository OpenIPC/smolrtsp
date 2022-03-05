#include <smolrtsp/types/response_line.h>

#include "../macros.h"
#include "parsing.h"
#include <smolrtsp/util.h>

#include <assert.h>
#include <string.h>

ssize_t SmolRTSP_ResponseLine_serialize(
    const SmolRTSP_ResponseLine *restrict self, SmolRTSP_Writer w) {
    assert(self);
    assert(w.self && w.vptr);

    ssize_t result = 0;

    CHK_WRITE_ERR(result, SmolRTSP_RtspVersion_serialize(&self->version, w));
    CHK_WRITE_ERR(result, VCALL(w, write, CharSlice99_from_str(" ")));
    CHK_WRITE_ERR(result, SmolRTSP_StatusCode_serialize(&self->code, w));
    CHK_WRITE_ERR(result, VCALL(w, write, CharSlice99_from_str(" ")));
    CHK_WRITE_ERR(result, VCALL(w, write, self->reason));
    CHK_WRITE_ERR(result, VCALL(w, write, SMOLRTSP_CRLF));

    return result;
}

SmolRTSP_ParseResult SmolRTSP_ResponseLine_parse(
    SmolRTSP_ResponseLine *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(SmolRTSP_RtspVersion_parse(&self->version, input));
    MATCH(SmolRTSP_StatusCode_parse(&self->code, input));
    MATCH(SmolRTSP_ReasonPhrase_parse(&self->reason, input));

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_ResponseLine_eq(
    const SmolRTSP_ResponseLine *restrict lhs,
    const SmolRTSP_ResponseLine *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return SmolRTSP_RtspVersion_eq(&lhs->version, &rhs->version) &&
           lhs->code == rhs->code &&
           SmolRTSP_ReasonPhrase_eq(&lhs->reason, &rhs->reason);
}
