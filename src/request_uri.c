#include <smolrtsp/request_uri.h>

#include <assert.h>
#include <string.h>

void SmolRTSP_RequestURI_serialize(
    const SmolRTSP_RequestURI *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    user_writer(strlen(self->data), self->data, user_cx);
}

bool SmolRTSP_RequestURI_eq(const SmolRTSP_RequestURI *lhs, const SmolRTSP_RequestURI *rhs) {
    assert(lhs);
    assert(rhs);

    return strncmp(lhs->data, rhs->data, SMOLRTSP_REQUEST_URI_SIZE) == 0;
}
