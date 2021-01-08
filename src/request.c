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
    SmolRTSP_RequestDeserializerState *restrict state) {
    precondition(self);
    precondition(data);
    precondition(bytes_read);
    precondition(state);

    // TODO: Make a eDSL for this shit.
    if (state->in_progress == SmolRTSP_RequestDeserializerStateRequestLine) {
        MATCH(SmolRTSP_RequestLine_deserialize(
            &self->start_line, data, bytes_read, &state->start_line));
        state->in_progress = SmolRTSP_RequestDeserializerStateHeaderMap;
    }

    if (state->in_progress == SmolRTSP_RequestDeserializerStateHeaderMap) {
        MATCH(SmolRTSP_HeaderMap_deserialize(&self->header_map, data, bytes_read));
        state->in_progress = SmolRTSP_RequestDeserializerStateMessageBody;
    }

    bool is_found;
    Slice99 content_length_slice =
        SmolRTSP_HeaderMap_find(self->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH, &is_found);

    size_t content_length;
    if (!is_found) {
        content_length = 0;
    } else if (sscanf(content_length_slice.ptr, "%zd", &content_length) != 1) {
        // TODO: Handle this error in a proper way.
        abort();
    }

    if (state->in_progress == SmolRTSP_RequestDeserializerStateMessageBody) {
        MATCH(SmolRTSP_MessageBody_deserialize(&self->body, data, bytes_read, content_length));
        state->in_progress = SmolRTSP_RequestDeserializerStateDone;
    }

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
