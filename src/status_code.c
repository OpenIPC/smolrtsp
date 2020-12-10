#include <smolrtsp/status_code.h>

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void SmolRTSP_StatusCode_serialize(
    const SmolRTSP_StatusCode *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%" PRIuLEAST16, *self);
    user_writer(strlen(buffer), buffer, user_cx);
}
