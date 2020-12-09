#include <smolrtsp/message_body.h>

#include <string.h>

SmolRTSP_MessageBody SmolRTSP_MessageBody_empty(void) {
    return (SmolRTSP_MessageBody){.size = 0, .data = NULL};
}

void SmolRTSP_MessageBody_serialize(
    const SmolRTSP_MessageBody *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(self->size, self->data, user_cx);
}

bool SmolRTSP_MessageBody_eq(const SmolRTSP_MessageBody *lhs, const SmolRTSP_MessageBody *rhs) {
    if (lhs->data == NULL && rhs->data == NULL) {
        return true;
    }

    if (lhs->size != rhs->size) {
        return false;
    }

    size_t size = lhs->size;
    return memcmp(lhs->data, rhs->data, size);
}
