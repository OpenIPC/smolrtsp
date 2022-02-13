#include <smolrtsp/types/response.h>

#include "parsing.h"

#include <assert.h>
#include <stdlib.h>

void SmolRTSP_Response_serialize(SmolRTSP_Response self, SmolRTSP_Writer w, void *w_ctx) {
    assert(w);

    SmolRTSP_ResponseLine_serialize(self.start_line, w, w_ctx);
    SmolRTSP_HeaderMap_serialize(self.header_map, w, w_ctx);
    w(self.body, w_ctx);
}

SmolRTSP_ParseResult SmolRTSP_Response_parse(
    SmolRTSP_Response *restrict self, CharSlice99 input,
    SmolRTSP_ResponseParseState *restrict state) {
    assert(self);
    assert(state);

    const CharSlice99 backup = input;

    TRY_PARSE(
        SmolRTSP_ResponseParseState_ResponseLine,
        SmolRTSP_ResponseLine_parse(&self->start_line, input, &state->start_line));

    TRY_PARSE(
        SmolRTSP_ResponseParseState_HeaderMap, SmolRTSP_HeaderMap_parse(&self->header_map, input));

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
        SmolRTSP_ResponseParseState_MessageBody,
        SmolRTSP_MessageBody_parse(&self->body, input, content_length_int));

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_Response_eq(SmolRTSP_Response lhs, SmolRTSP_Response rhs) {
    return SmolRTSP_ResponseLine_eq(lhs.start_line, rhs.start_line) &&
           SmolRTSP_HeaderMap_eq(lhs.header_map, rhs.header_map) &&
           CharSlice99_primitive_eq(lhs.body, rhs.body);
}
