#include "correctness.h"
#include <smolrtsp/request.h>

void SmolRTSP_Response_serialize(
    const SmolRTSP_Request *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    SmolRTSP_RequestLine_serialize(&self->start_line, user_writer, user_cx);
    SmolRTSP_HeaderMap_serialize(&self->header_map, user_writer, user_cx);
    user_writer(self->body.len, self->body.ptr, user_cx);
}

bool SmolRTSP_Request_eq(
    const SmolRTSP_Request *restrict lhs, const SmolRTSP_Request *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return SmolRTSP_RequestLine_eq(&lhs->start_line, &rhs->start_line) &&
           SmolRTSP_HeaderMap_eq(&lhs->header_map, &rhs->header_map) &&
           Slice99_primitive_eq(lhs->body, rhs->body);
}
