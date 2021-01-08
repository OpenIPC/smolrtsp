#include "correctness.h"
#include "parsing.h"
#include <smolrtsp/status_code.h>

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void SmolRTSP_StatusCode_serialize(
    SmolRTSP_StatusCode self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%" PRIuLEAST16, self);
    user_writer(strlen(buffer), buffer, user_cx);
}

SmolRTSP_DeserializeResult
SmolRTSP_StatusCode_deserialize(SmolRTSP_StatusCode *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    MATCH(SmolRTSP_match_whitespaces(data));
    Slice99 code = *data;
    MATCH(SmolRTSP_match_numeric(data));
    code = Slice99_from_ptrdiff(code.ptr, data->ptr, sizeof(char));

    SmolRTSP_StatusCode code_int;
    char format[50];
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, code.len);
    if (sscanf(code.ptr, format, &code_int) != 1) {
        return SmolRTSP_DeserializeResultErr;
    }

    *self = code_int;

    return SmolRTSP_DeserializeResultOk;
}
