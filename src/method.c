#include <smolrtsp/method.h>

#include <assert.h>
#include <string.h>

void SmolRTSP_Method_serialize(
    const SmolRTSP_Method *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    user_writer(strlen(self->data), self->data, user_cx);
}

bool SmolRTSP_Method_eq(const SmolRTSP_Method *lhs, const SmolRTSP_Method *rhs) {
    assert(lhs);
    assert(rhs);

    return strncmp(lhs->data, rhs->data, SMOLRTSP_METHOD_SIZE) == 0;
}
