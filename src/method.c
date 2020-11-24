#include <smolrtsp/method.h>

#include <string.h>

void SmolRTSP_Method_serialize(
    const SmolRTSP_Method *restrict method, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(strlen(method), (const void *)*method, user_cx);
}
