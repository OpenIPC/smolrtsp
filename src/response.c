#include <smolrtsp/response.h>

#include "correctness.h"
#include "parsing.h"

#include <stdlib.h>

void SmolRTSP_Response_serialize(
    SmolRTSP_Response self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    SmolRTSP_ResponseLine_serialize(self.start_line, user_writer, user_cx);
    SmolRTSP_HeaderMap_serialize(self.header_map, user_writer, user_cx);
    user_writer(self.body, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_Response_deserialize(
    SmolRTSP_Response *restrict self, Slice99 *restrict data,
    SmolRTSP_ResponseDeserializerState *restrict state) {
    precondition(self);
    precondition(data);
    precondition(state);

    TRY_PARSE(
        SmolRTSP_ResponseDeserializerStateResponseLine,
        SmolRTSP_ResponseLine_deserialize(&self->start_line, data, &state->start_line));

    TRY_PARSE(
        SmolRTSP_ResponseDeserializerStateHeaderMap,
        SmolRTSP_HeaderMap_deserialize(&self->header_map, data));

    Slice99Maybe content_length =
        SmolRTSP_HeaderMap_find(self->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH);

    size_t content_length_int = 0;
    if (content_length.exists) {
        char fmt[64];
        snprintf(fmt, sizeof(fmt), "%%%zdzd", content_length.slice.len);

        if (sscanf(content_length.slice.ptr, fmt, &content_length_int) != 1) {
            // TODO: Handle this error in a proper way.
            abort();
        }
    }

    TRY_PARSE(
        SmolRTSP_ResponseDeserializerStateMessageBody,
        SmolRTSP_MessageBody_deserialize(&self->body, data, content_length_int));

    return SmolRTSP_DeserializeResultOk;
}

bool SmolRTSP_Response_eq(SmolRTSP_Response lhs, SmolRTSP_Response rhs) {
    return SmolRTSP_ResponseLine_eq(lhs.start_line, rhs.start_line) &&
           SmolRTSP_HeaderMap_eq(lhs.header_map, rhs.header_map) &&
           Slice99_primitive_eq(lhs.body, rhs.body);
}
