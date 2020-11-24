#include <smolrtsp/request_uri.h>

#include <string.h>

void SmolRTSP_RequestURI_serialize(
    const SmolRTSP_RequestURI *restrict uri, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(strlen(uri), (const void *)*uri, user_cx);
}
