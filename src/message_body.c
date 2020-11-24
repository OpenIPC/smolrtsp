#include <smolrtsp/message_body.h>

void SmolRTSP_MessageBody_serialize(
    const SmolRTSP_MessageBody *restrict body, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(body->size, body->data, user_cx);
}
