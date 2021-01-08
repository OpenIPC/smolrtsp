#include "correctness.h"
#include <smolrtsp/status_code.h>
#include "match.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void SmolRTSP_StatusCode_serialize(
    const SmolRTSP_StatusCode *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%" PRIuLEAST16, *self);
    user_writer(strlen(buffer), buffer, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_StatusCode_deserialize(
    SmolRTSP_StatusCode *restrict self, Slice99 *restrict data, size_t *restrict bytes_read) {
    precondition(self);
    precondition(data);
    precondition(bytes_read);

    size_t bytes_read_temp = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read_temp));
    const char *code = data->ptr;
    MATCH(SmolRTSP_match_numeric(data, &bytes_read_temp));
    const size_t code_size = (const char *)data->ptr - code;

    SmolRTSP_StatusCode code_int;
    char format[50];
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, code_size);
    int rc = sscanf(code, format, &code_int);
    precondition(rc == 1);

    *bytes_read = bytes_read_temp;
    *self = code_int;

    return SmolRTSP_DeserializeResultOk;
}
