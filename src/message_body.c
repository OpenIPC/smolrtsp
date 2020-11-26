#include <smolrtsp/message_body.h>

void SmolRTSP_MessageBody_serialize(
    const SmolRTSP_MessageBody *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(self->size, self->data, user_cx);
}
