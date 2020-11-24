#include <smolrtsp/status_code.h>

#include <inttypes.h>
#include <stdio.h>

void SmolRTSP_StatusCode_serialize(
    const SmolRTSP_StatusCode *restrict code, SmolRTSP_UserWriter user_writer, void *user_cx) {
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%" PRIuLEAST16, *code);
    user_writer(strlen(buffer), buffer, user_cx);
}
