#include <smolrtsp/method.h>

#include <string.h>

void SmolRTSP_Method_serialize(
    const SmolRTSP_Method *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(strlen(self->data), self->data, user_cx);
}
