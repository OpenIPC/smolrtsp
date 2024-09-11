#include <smolrtsp/types/response.h>
#include <smolrtsp/util.h>

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

ssize_t SmolRTSP_Response_serialize(
    const SmolRTSP_Response *restrict self, SmolRTSP_Writer w) {
    assert(self);
    assert(w.self && w.vptr);

    ssize_t result = 0;

    CHK_WRITE_ERR(
        result, SmolRTSP_ResponseLine_serialize(&self->start_line, w));

    if (!SmolRTSP_HeaderMap_contains_key(
            &self->header_map, SMOLRTSP_HEADER_C_SEQ)) {
        const SmolRTSP_Header cseq = {
            SMOLRTSP_HEADER_C_SEQ,
            CharSlice99_alloca_fmt("%" PRIu32, self->cseq),
        };
        CHK_WRITE_ERR(result, SmolRTSP_Header_serialize(&cseq, w));
    }

    if (!SmolRTSP_HeaderMap_contains_key(
            &self->header_map, SMOLRTSP_HEADER_CONTENT_LENGTH) &&
        !CharSlice99_is_empty(self->body)) {
        const SmolRTSP_Header content_length = {
            SMOLRTSP_HEADER_CONTENT_LENGTH,
            CharSlice99_alloca_fmt("%zd", self->body.len),
        };
        CHK_WRITE_ERR(result, SmolRTSP_Header_serialize(&content_length, w));
    }

    CHK_WRITE_ERR(result, SmolRTSP_HeaderMap_serialize(&self->header_map, w));
    if (!CharSlice99_is_empty(self->body)) {
        CHK_WRITE_ERR(result, VCALL(w, write, self->body));
    }

    return result;
}

SmolRTSP_ParseResult
SmolRTSP_Response_parse(SmolRTSP_Response *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    // TODO: implement proper parsing of interleaved binary data.
    for (;;) {
        if (input.len < sizeof(uint32_t)) {
            return SmolRTSP_ParseResult_partial();
        }

        if ('$' == input.ptr[0]) {
            uint8_t channel_id = 0;
            uint16_t payload_len = 0;
            smolrtsp_parse_interleaved_header(
                (const uint8_t *)input.ptr, &channel_id, &payload_len);

            input = CharSlice99_advance(input, sizeof(uint32_t));

            if (input.len < (size_t)payload_len) {
                return SmolRTSP_ParseResult_partial();
            }
            input = CharSlice99_advance(input, (size_t)payload_len);
        } else {
            break;
        }
    }

    MATCH(SmolRTSP_ResponseLine_parse(&self->start_line, input));
    MATCH(SmolRTSP_HeaderMap_parse(&self->header_map, input));

    CharSlice99 content_length;
    size_t content_length_int = 0;
    const bool content_length_is_found = SmolRTSP_HeaderMap_find(
        &self->header_map, SMOLRTSP_HEADER_CONTENT_LENGTH, &content_length);

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
        &self->header_map, SMOLRTSP_HEADER_C_SEQ, &cseq_value);
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

bool SmolRTSP_Response_eq(
    const SmolRTSP_Response *restrict lhs,
    const SmolRTSP_Response *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return SmolRTSP_ResponseLine_eq(&lhs->start_line, &rhs->start_line) &&
           SmolRTSP_HeaderMap_eq(&lhs->header_map, &rhs->header_map) &&
           SmolRTSP_MessageBody_eq(&lhs->body, &rhs->body) &&
           lhs->cseq == rhs->cseq;
}
