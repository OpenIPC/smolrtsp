#include "match.h"
#include <smolrtsp/crlf.h>
#include <smolrtsp/header_map.h>

#include <assert.h>
#include <string.h>

SmolRTSP_Slice SmolRTSP_HeaderMap_find(SmolRTSP_HeaderMap *restrict self, SmolRTSP_Slice key) {
    assert(self);
    assert(!SmolRTSP_Slice_is_null(key));

    for (size_t i = 0; i < self->len; i++) {
        if (SmolRTSP_Slice_eq(&self->headers[i].key, &key)) {
            return self->headers[i].value;
        }
    }

    return SmolRTSP_Slice_null();
}

void SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    for (size_t i = 0; i < self->len; i++) {
        SmolRTSP_Header_serialize(&self->headers[i], user_writer, user_cx);
    }

    user_writer(strlen(SMOLRTSP_CRLF), SMOLRTSP_CRLF, user_cx);
}

bool SmolRTSP_HeaderMap_eq(
    const SmolRTSP_HeaderMap *restrict lhs, const SmolRTSP_HeaderMap *restrict rhs) {
    assert(lhs);
    assert(rhs);

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

void SmolRTSP_HeaderMap_pretty_print_to_file(const SmolRTSP_HeaderMap *self, FILE *stream) {
    assert(self);
    assert(stream);

    for (size_t i = 0; i < self->len; i++) {
        SmolRTSP_Header_pretty_print_to_file(&self->headers[i], stream);
    }
}

void SmolRTSP_HeaderMap_pretty_print(const SmolRTSP_HeaderMap *self) {
    assert(self);

    SmolRTSP_HeaderMap_pretty_print_to_file(self, stdout);
}
