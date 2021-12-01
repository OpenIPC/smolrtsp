#include <smolrtsp/status_code.h>

#include "parsing.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void SmolRTSP_StatusCode_serialize(
    SmolRTSP_StatusCode self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(user_writer);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%" PRIuLEAST16, self);
    user_writer(CharSlice99_from_str(buffer), user_cx);
}

SmolRTSP_ParseResult
SmolRTSP_StatusCode_parse(SmolRTSP_StatusCode *restrict self, CharSlice99 *restrict data) {
    assert(self);
    assert(data);

    MATCH(smolrtsp_match_whitespaces(data));
    CharSlice99 code = *data;
    MATCH(smolrtsp_match_numeric(data));
    code = CharSlice99_from_ptrdiff(code.ptr, data->ptr);

    SmolRTSP_StatusCode code_int;
    char fmt[64];
    snprintf(fmt, sizeof(fmt), "%%%zd" SCNuLEAST16, code.len);
    if (sscanf(code.ptr, fmt, &code_int) != 1) {
        return SmolRTSP_ParseResult_Err;
    }

    *self = code_int;

    return SmolRTSP_ParseResult_Ok;
}
