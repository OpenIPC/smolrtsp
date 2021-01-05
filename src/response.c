#include "correctness.h"
#include <smolrtsp/response.h>

void SmolRTSP_Response_serialize(
    const SmolRTSP_Response *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_cx);

    SmolRTSP_ResponseLine_serialize(&self->start_line, user_writer, user_cx);
    SmolRTSP_HeaderMap_serialize(&self->header_map, user_writer, user_cx);
    user_writer(self->body.len, self->body.ptr, user_cx);
}

bool SmolRTSP_Response_eq(
    const SmolRTSP_Response *restrict lhs, const SmolRTSP_Response *restrict rhs) {
    return SmolRTSP_ResponseLine_eq(&lhs->start_line, &rhs->start_line) &&
           SmolRTSP_HeaderMap_eq(&lhs->header_map, &rhs->header_map) &&
           Slice99_primitive_eq(lhs->body, rhs->body);
}
