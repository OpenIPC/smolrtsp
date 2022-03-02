#include <smolrtsp/types/request.h>

#include "../macros.h"
#include "parsing.h"

#include <assert.h>
#include <inttypes.h>

#include <alloca.h>

ssize_t SmolRTSP_Request_serialize(SmolRTSP_Request self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    ssize_t result = 0;

    CHK_WRITE_ERR(result, SmolRTSP_RequestLine_serialize(self.start_line, w));
    CHK_WRITE_ERR(result, SmolRTSP_HeaderMap_serialize(self.header_map, w));
    CHK_WRITE_ERR(result, VCALL(w, write, self.body));

    return result;
}

SmolRTSP_ParseResult
SmolRTSP_Request_parse(SmolRTSP_Request *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(SmolRTSP_RequestLine_parse(&self->start_line, input));
    MATCH(SmolRTSP_HeaderMap_parse(&self->header_map, input));

    CharSlice99 content_length;
    size_t content_length_int = 0;
    const bool content_length_found = SmolRTSP_HeaderMap_find(
        self->header_map, SMOLRTSP_HEADER_CONTENT_LENGTH, &content_length);

    if (content_length_found) {
        char fmt[64];
        snprintf(fmt, sizeof(fmt), "%%%zdzd", content_length.len);

        if (sscanf(content_length.ptr, fmt, &content_length_int) != 1) {
            return SmolRTSP_ParseResult_Failure(
                SmolRTSP_ParseError_ContentLength(content_length));
        }
    }

    MATCH(SmolRTSP_MessageBody_parse(&self->body, input, content_length_int));

    CharSlice99 cseq_value;
    const bool cseq_found = SmolRTSP_HeaderMap_find(
        self->header_map, SMOLRTSP_HEADER_C_SEQ, &cseq_value);
    if (!cseq_found) {
        return SmolRTSP_ParseResult_Failure(SmolRTSP_ParseError_MissingCSeq());
    }

    uint32_t cseq;
    if (sscanf(CharSlice99_alloca_c_str(cseq_value), "%" SCNu32, &cseq) != 1) {
        return SmolRTSP_ParseResult_Failure(
            SmolRTSP_ParseError_InvalidCSeq(cseq_value));
    }

    self->cseq = cseq;

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs) {
    return SmolRTSP_RequestLine_eq(lhs.start_line, rhs.start_line) &&
           SmolRTSP_HeaderMap_eq(lhs.header_map, rhs.header_map) &&
           CharSlice99_primitive_eq(lhs.body, rhs.body) && lhs.cseq == rhs.cseq;
}
