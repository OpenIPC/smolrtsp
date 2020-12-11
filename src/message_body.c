#include <smolrtsp/message_body.h>

#include <assert.h>
#include <string.h>

void SmolRTSP_MessageBody_serialize(
    const SmolRTSP_MessageBody *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    SmolRTSP_Slice_serialize(self, user_writer, user_cx);
}

bool SmolRTSP_MessageBody_eq(const SmolRTSP_MessageBody *lhs, const SmolRTSP_MessageBody *rhs) {
    assert(lhs);
    assert(rhs);

    return SmolRTSP_Slice_eq(lhs, rhs);
}
