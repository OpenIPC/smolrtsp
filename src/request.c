#include "correctness.h"
#include <smolrtsp/request.h>

bool SmolRTSP_Request_eq(
    const SmolRTSP_Request *restrict lhs, const SmolRTSP_Request *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return SmolRTSP_RequestLine_eq(&lhs->start_line, &rhs->start_line) &&
           SmolRTSP_HeaderMap_eq(&lhs->header_map, &rhs->header_map) &&
           SmolRTSP_Slice_eq(&lhs->body, &rhs->body);
}
