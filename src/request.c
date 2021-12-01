#include <smolrtsp/request.h>

#include "parsing.h"

#include <assert.h>
#include <stdlib.h>

void SmolRTSP_Request_serialize(
    SmolRTSP_Request self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(user_writer);

    SmolRTSP_RequestLine_serialize(self.start_line, user_writer, user_cx);
    SmolRTSP_HeaderMap_serialize(self.header_map, user_writer, user_cx);
    user_writer(self.body, user_cx);
}

SmolRTSP_ParseResult SmolRTSP_Request_parse(
    SmolRTSP_Request *restrict self, CharSlice99 *restrict data,
    SmolRTSP_RequestDeserializerState *restrict state) {
    assert(self);
    assert(data);
    assert(state);

    TRY_PARSE(
        SmolRTSP_RequestDeserializerState_RequestLine,
        SmolRTSP_RequestLine_parse(&self->start_line, data, &state->start_line));

    TRY_PARSE(
        SmolRTSP_RequestDeserializerState_HeaderMap,
        SmolRTSP_HeaderMap_parse(&self->header_map, data));

    CharSlice99 content_length;
    size_t content_length_int = 0;
    const bool content_length_is_found = SmolRTSP_HeaderMap_find(
        self->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH, &content_length);

    if (content_length_is_found) {
        char fmt[64];
        snprintf(fmt, sizeof(fmt), "%%%zdzd", content_length.len);

        if (sscanf(content_length.ptr, fmt, &content_length_int) != 1) {
            // TODO: Handle this error in a proper way.
            abort();
        }
    }

    TRY_PARSE(
        SmolRTSP_RequestDeserializerState_MessageBody,
        SmolRTSP_MessageBody_parse(&self->body, data, content_length_int));

    return SmolRTSP_ParseResult_Ok;
}

bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs) {
    return SmolRTSP_RequestLine_eq(lhs.start_line, rhs.start_line) &&
           SmolRTSP_HeaderMap_eq(lhs.header_map, rhs.header_map) &&
           CharSlice99_primitive_eq(lhs.body, rhs.body);
}
