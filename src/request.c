#include <smolrtsp/request.h>

bool SmolRTSP_Request_eq(const SmolRTSP_Request *lhs, const SmolRTSP_Request *rhs) {
    return SmolRTSP_RequestLine_eq(&lhs->start_line, &rhs->start_line) &&
           SmolRTSP_HeaderMap_eq(&lhs->header_map, &rhs->header_map) &&
           SmolRTSP_MessageBody_eq(&lhs->body, &rhs->body);
}
