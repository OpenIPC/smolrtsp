#include <smolrtsp/request_uri.h>

#include <string.h>

void SmolRTSP_RequestURI_serialize(
    const SmolRTSP_RequestURI *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(strlen(*self), *self, user_cx);
}
