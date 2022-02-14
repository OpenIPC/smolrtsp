#include <smolrtsp/types/request.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <stdlib.h>

ssize_t SmolRTSP_Request_serialize(SmolRTSP_Request self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    ssize_t result = 0, ret = 0;

    ret = SmolRTSP_RequestLine_serialize(self.start_line, w);
    CHK_WRITE_ERR(result, ret);
    ret = SmolRTSP_HeaderMap_serialize(self.header_map, w);
    CHK_WRITE_ERR(result, ret);
    ret = VCALL(w, write, self.body);
    CHK_WRITE_ERR(result, ret);

    return result;
}

SmolRTSP_ParseResult SmolRTSP_Request_parse(
    SmolRTSP_Request *restrict self, CharSlice99 input,
    SmolRTSP_RequestParseState *restrict state) {
    assert(self);
    assert(state);

    const CharSlice99 backup = input;

    TRY_PARSE(
        SmolRTSP_RequestParseState_RequestLine,
        SmolRTSP_RequestLine_parse(&self->start_line, input));

    TRY_PARSE(
        SmolRTSP_RequestParseState_HeaderMap, SmolRTSP_HeaderMap_parse(&self->header_map, input));

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
        SmolRTSP_RequestParseState_MessageBody,
        SmolRTSP_MessageBody_parse(&self->body, input, content_length_int));

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs) {
    return SmolRTSP_RequestLine_eq(lhs.start_line, rhs.start_line) &&
           SmolRTSP_HeaderMap_eq(lhs.header_map, rhs.header_map) &&
           CharSlice99_primitive_eq(lhs.body, rhs.body);
}
