#include <smolrtsp/method.h>

#include <string.h>

void SmolRTSP_Method_serialize(
    const SmolRTSP_Method *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(strlen(self->data), self->data, user_cx);
}

bool SmolRTSP_Method_eq(const SmolRTSP_Method *lhs, const SmolRTSP_Method *rhs) {
    return strncmp(lhs->data, rhs->data, SMOLRTSP_METHOD_SIZE) == 0;
}
