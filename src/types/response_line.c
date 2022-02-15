#include <smolrtsp/types/response_line.h>

#include "../macros.h"
#include "parsing.h"
#include <smolrtsp/util.h>

#include <assert.h>
#include <string.h>

ssize_t SmolRTSP_ResponseLine_serialize(SmolRTSP_ResponseLine self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    ssize_t result = 0, ret = 0;

    ret = SmolRTSP_RtspVersion_serialize(self.version, w);
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, CharSlice99_from_str(" "));
    CHK_WRITE_ERR(result, ret);
    ret = SmolRTSP_StatusCode_serialize(self.code, w);
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, CharSlice99_from_str(" "));
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, self.reason);
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, SMOLRTSP_CRLF);
    CHK_WRITE_ERR(result, ret);

    return result;
}

SmolRTSP_ParseResult
SmolRTSP_ResponseLine_parse(SmolRTSP_ResponseLine *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(SmolRTSP_RtspVersion_parse(&self->version, input));
    MATCH(SmolRTSP_StatusCode_parse(&self->code, input));
    MATCH(SmolRTSP_ReasonPhrase_parse(&self->reason, input));

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_ResponseLine_eq(SmolRTSP_ResponseLine lhs, SmolRTSP_ResponseLine rhs) {
    return SmolRTSP_RtspVersion_eq(lhs.version, rhs.version) && lhs.code == rhs.code &&
           CharSlice99_primitive_eq(lhs.reason, rhs.reason);
}
