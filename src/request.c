#include "correctness.h"
#include "parsing.h"
#include <smolrtsp/request.h>

void SmolRTSP_Request_serialize(
    SmolRTSP_Request self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    SmolRTSP_RequestLine_serialize(self.start_line, user_writer, user_cx);
    SmolRTSP_HeaderMap_serialize(self.header_map, user_writer, user_cx);
    user_writer(self.body, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_Request_deserialize(
    SmolRTSP_Request *restrict self, Slice99 *restrict data,
    SmolRTSP_RequestDeserializerState *restrict state) {
    precondition(self);
    precondition(data);
    precondition(state);

    TRY_PARSE(
        SmolRTSP_RequestDeserializerStateRequestLine,
        SmolRTSP_RequestLine_deserialize(&self->start_line, data, &state->start_line));

    TRY_PARSE(
        SmolRTSP_RequestDeserializerStateHeaderMap,
        SmolRTSP_HeaderMap_deserialize(&self->header_map, data));

    Slice99Maybe content_length =
        SmolRTSP_HeaderMap_find(self->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH);

    size_t content_length_int = 0;
    if (content_length.exists) {
        char fmt[50];
        snprintf(fmt, sizeof(fmt), "%%%zdzd", content_length.slice.len);

        if (sscanf(content_length.slice.ptr, fmt, &content_length_int) != 1) {
            // TODO: Handle this error in a proper way.
            abort();
        }
    }

    TRY_PARSE(
        SmolRTSP_RequestDeserializerStateMessageBody,
        SmolRTSP_MessageBody_deserialize(&self->body, data, content_length_int));

    return SmolRTSP_DeserializeResultOk;
}

bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs) {
    return SmolRTSP_RequestLine_eq(lhs.start_line, rhs.start_line) &&
           SmolRTSP_HeaderMap_eq(lhs.header_map, rhs.header_map) &&
           Slice99_primitive_eq(lhs.body, rhs.body);
}
