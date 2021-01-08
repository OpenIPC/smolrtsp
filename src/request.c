#include "correctness.h"
#include "match.h"
#include <smolrtsp/request.h>

void SmolRTSP_Response_serialize(
    const SmolRTSP_Request *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    SmolRTSP_RequestLine_serialize(&self->start_line, user_writer, user_cx);
    SmolRTSP_HeaderMap_serialize(&self->header_map, user_writer, user_cx);
    user_writer(self->body.len, self->body.ptr, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_Request_deserialize(
    SmolRTSP_Request *restrict self, Slice99 *restrict data, size_t *restrict bytes_read,
    SmolRTSP_RequestDeserializerState *restrict state,
    SmolRTSP_RequestLineDeserializerState *restrict start_line_state) {
    precondition(self);
    precondition(data);
    precondition(bytes_read);
    precondition(state);

#define TRY_PARSE(if_state, expr)                                                                  \
    do {                                                                                           \
        if (*state == if_state) {                                                                  \
            MATCH(expr);                                                                           \
            (*state)++;                                                                            \
        }                                                                                          \
    } while (0)

    TRY_PARSE(
        SmolRTSP_RequestDeserializerStateRequestLine,
        SmolRTSP_RequestLine_deserialize(&self->start_line, data, bytes_read, start_line_state));

    TRY_PARSE(
        SmolRTSP_RequestDeserializerStateHeaderMap,
        SmolRTSP_HeaderMap_deserialize(&self->header_map, data, bytes_read));

    bool is_found;
    Slice99 content_length_slice =
        SmolRTSP_HeaderMap_find(self->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH, &is_found);

    size_t content_length = 0;
    if (is_found) {
        if (sscanf(content_length_slice.ptr, "%zd", &content_length) != 1) {
            // TODO: Handle this error in a proper way.
            abort();
        }
    }

    TRY_PARSE(
        SmolRTSP_RequestDeserializerStateMessageBody,
        SmolRTSP_MessageBody_deserialize(&self->body, data, bytes_read, content_length));

#undef TRY_PARSE

    return SmolRTSP_DeserializeResultOk;
}

bool SmolRTSP_Request_eq(
    const SmolRTSP_Request *restrict lhs, const SmolRTSP_Request *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return SmolRTSP_RequestLine_eq(&lhs->start_line, &rhs->start_line) &&
           SmolRTSP_HeaderMap_eq(&lhs->header_map, &rhs->header_map) &&
           Slice99_primitive_eq(lhs->body, rhs->body);
}
