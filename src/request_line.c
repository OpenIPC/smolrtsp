#include "aux.h"
#include <smolrtsp/deserializers/method.h>
#include <smolrtsp/deserializers/request_uri.h>
#include <smolrtsp/deserializers/rtsp_version.h>
#include <smolrtsp/request_line.h>

#include <assert.h>
#include <string.h>

void SmolRTSP_RequestLine_serialize(
    const SmolRTSP_RequestLine *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    SmolRTSP_Method_serialize(&self->method, user_writer, user_cx);
    SmolRTSP_RequestURI_serialize(&self->uri, user_writer, user_cx);
    SmolRTSP_RTSPVersion_serialize(&self->version, user_writer, user_cx);
    user_writer(strlen(CRLF), CRLF, user_cx);
}

bool SmolRTSP_RequestLine_eq(const SmolRTSP_RequestLine *lhs, const SmolRTSP_RequestLine *rhs) {
    assert(lhs);
    assert(rhs);

    return SmolRTSP_Method_eq(&lhs->method, &rhs->method) &&
           SmolRTSP_RequestURI_eq(&lhs->uri, &rhs->uri) &&
           SmolRTSP_RTSPVersion_eq(&lhs->version, &rhs->version);
}
