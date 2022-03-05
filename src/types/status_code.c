#include <smolrtsp/types/status_code.h>

#include "parsing.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <alloca.h>

ssize_t SmolRTSP_StatusCode_serialize(
    const SmolRTSP_StatusCode *restrict self, SmolRTSP_Writer w) {
    assert(self);
    assert(w.self && w.vptr);

    return VCALL(w, writef, "%" PRIu16, *self);
}

SmolRTSP_ParseResult SmolRTSP_StatusCode_parse(
    SmolRTSP_StatusCode *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(smolrtsp_match_whitespaces(input));
    CharSlice99 code = input;
    MATCH(smolrtsp_match_numeric(input));
    code = CharSlice99_from_ptrdiff(code.ptr, input.ptr);

    SmolRTSP_StatusCode code_int;
    if (sscanf(CharSlice99_alloca_c_str(code), "%" SCNu16, &code_int) != 1) {
        return SmolRTSP_ParseResult_Failure(
            SmolRTSP_ParseError_TypeMismatch(SmolRTSP_ParseType_Int, code));
    }

    *self = code_int;

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_StatusCode_eq(
    const SmolRTSP_StatusCode *restrict lhs,
    const SmolRTSP_StatusCode *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return *lhs == *rhs;
}
