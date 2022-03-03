#include <smolrtsp/types/response.h>

#include "../macros.h"
#include "parsing.h"

#include <assert.h>
#include <inttypes.h>

#include <alloca.h>

#include <slice99.h>

SmolRTSP_Response SmolRTSP_Response_uninit(void) {
    SmolRTSP_Response self;
    memset(&self, '\0', sizeof self);
    self.header_map = SmolRTSP_HeaderMap_empty();
    return self;
}

ssize_t SmolRTSP_Response_serialize(SmolRTSP_Response self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    ssize_t result = 0;

    CHK_WRITE_ERR(result, SmolRTSP_ResponseLine_serialize(self.start_line, w));

    if (!SmolRTSP_HeaderMap_key_is_present(
            self.header_map, SMOLRTSP_HEADER_C_SEQ)) {
        const SmolRTSP_Header cseq = {
            SMOLRTSP_HEADER_C_SEQ,
            CharSlice99_alloca_fmt("%" PRIu32, self.cseq),
        };
        CHK_WRITE_ERR(result, SmolRTSP_Header_serialize(cseq, w));
    }

    if (!SmolRTSP_HeaderMap_key_is_present(
            self.header_map, SMOLRTSP_HEADER_CONTENT_LENGTH) &&
        !CharSlice99_is_empty(self.body)) {
        const SmolRTSP_Header content_length = {
            SMOLRTSP_HEADER_CONTENT_LENGTH,
            CharSlice99_alloca_fmt("%zd", self.body.len),
        };
        CHK_WRITE_ERR(result, SmolRTSP_Header_serialize(content_length, w));
    }

    CHK_WRITE_ERR(result, SmolRTSP_HeaderMap_serialize(self.header_map, w));
    if (!CharSlice99_is_empty(self.body)) {
        CHK_WRITE_ERR(result, VCALL(w, write, self.body));
    }

    return result;
}

SmolRTSP_ParseResult
SmolRTSP_Response_parse(SmolRTSP_Response *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(SmolRTSP_ResponseLine_parse(&self->start_line, input));
    MATCH(SmolRTSP_HeaderMap_parse(&self->header_map, input));

    CharSlice99 content_length;
    size_t content_length_int = 0;
    const bool content_length_is_found = SmolRTSP_HeaderMap_find(
        self->header_map, SMOLRTSP_HEADER_CONTENT_LENGTH, &content_length);

    if (content_length_is_found) {
        if (sscanf(
                CharSlice99_alloca_c_str(content_length), "%zd",
                &content_length_int) != 1) {
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

bool SmolRTSP_Response_eq(SmolRTSP_Response lhs, SmolRTSP_Response rhs) {
    return SmolRTSP_ResponseLine_eq(lhs.start_line, rhs.start_line) &&
           SmolRTSP_HeaderMap_eq(lhs.header_map, rhs.header_map) &&
           CharSlice99_primitive_eq(lhs.body, rhs.body) && lhs.cseq == rhs.cseq;
}

ssize_t smolrtsp_respond(
    SmolRTSP_Writer w, uint32_t cseq, SmolRTSP_StatusCode code,
    const char *reason, SmolRTSP_HeaderMap headers) {
    return smolrtsp_respond_with_body(
        w, cseq, code, reason, headers, SmolRTSP_MessageBody_empty());
}

ssize_t smolrtsp_respond_with_body(
    SmolRTSP_Writer w, uint32_t cseq, SmolRTSP_StatusCode code,
    const char *reason, SmolRTSP_HeaderMap headers, SmolRTSP_MessageBody body) {
    assert(w.self && w.vptr);
    assert(reason);

    const SmolRTSP_Response response = {
        .start_line =
            {
                .version = {.major = 1, .minor = 0},
                .code = code,
                .reason = CharSlice99_from_str((char *)reason),
            },
        .header_map = headers,
        .body = body,
        .cseq = cseq,
    };

    return SmolRTSP_Response_serialize(response, w);
}
