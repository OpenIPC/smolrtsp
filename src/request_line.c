#include "correctness.h"
#include <smolrtsp/deserializers/method.h>
#include <smolrtsp/deserializers/request_uri.h>
#include <smolrtsp/deserializers/rtsp_version.h>
#include <smolrtsp/request_line.h>

#include <string.h>

void SmolRTSP_RequestLine_serialize(
    const SmolRTSP_RequestLine *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    user_writer(Slice99_size(self->method), self->method.ptr, user_cx);
    user_writer(Slice99_size(self->uri), self->uri.ptr, user_cx);
    SmolRTSP_RTSPVersion_serialize(&self->version, user_writer, user_cx);
    user_writer(strlen(SMOLRTSP_CRLF), SMOLRTSP_CRLF, user_cx);
}

bool SmolRTSP_RequestLine_eq(
    const SmolRTSP_RequestLine *restrict lhs, const SmolRTSP_RequestLine *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return Slice99_primitive_eq(lhs->method, rhs->method) &&
           Slice99_primitive_eq(lhs->uri, rhs->uri) &&
           SmolRTSP_RTSPVersion_eq(&lhs->version, &rhs->version);
}
