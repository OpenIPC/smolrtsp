#include "correctness.h"
#include "match.h"
#include <smolrtsp/crlf.h>
#include <smolrtsp/header_map.h>

#include <string.h>

Slice99
SmolRTSP_HeaderMap_find(SmolRTSP_HeaderMap *restrict self, Slice99 key, bool *restrict is_found) {
    precondition(self);

    for (size_t i = 0; i < self->len; i++) {
        if (Slice99_primitive_eq(self->headers[i].key, key)) {
            *is_found = true;
            return self->headers[i].value;
        }
    }

    *is_found = false;
    return Slice99_empty(1);
}

void SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    for (size_t i = 0; i < self->len; i++) {
        SmolRTSP_Header_serialize(&self->headers[i], user_writer, user_cx);
    }

    user_writer(strlen(SMOLRTSP_CRLF), SMOLRTSP_CRLF, user_cx);
}

bool SmolRTSP_HeaderMap_eq(
    const SmolRTSP_HeaderMap *restrict lhs, const SmolRTSP_HeaderMap *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    if (lhs->len != rhs->len) {
        return false;
    }

    const size_t len = lhs->len;

    for (size_t i = 0; i < len; i++) {
        if (!SmolRTSP_Header_eq(&lhs->headers[i], &rhs->headers[i])) {
            return false;
        }
    }

    return true;
}

bool SmolRTSP_HeaderMap_is_full(const SmolRTSP_HeaderMap self) {
    return self.len == self.size;
}

void SmolRTSP_HeaderMap_dbg_to_file(const SmolRTSP_HeaderMap *self, FILE *stream) {
    precondition(self);
    precondition(stream);

    for (size_t i = 0; i < self->len; i++) {
        SmolRTSP_Header_dbg_to_file(&self->headers[i], stream);
    }
}

void SmolRTSP_HeaderMap_dbg(const SmolRTSP_HeaderMap *self) {
    precondition(self);

    SmolRTSP_HeaderMap_dbg_to_file(self, stdout);
}
