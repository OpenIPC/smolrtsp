#include "correctness.h"
#include "match.h"
#include <smolrtsp/crlf.h>
#include <smolrtsp/header_map.h>

#include <string.h>

static bool keys_and_values_are_not_null_slices(const SmolRTSP_HeaderMap *self) {
    precondition(self);

    for (size_t i = 0; i < self->len; i++) {
        if (SmolRTSP_Slice_is_null(self->headers[i].key) ||
            SmolRTSP_Slice_is_null(self->headers[i].value)) {
            return false;
        }
    }

    return true;
}

SmolRTSP_Slice SmolRTSP_HeaderMap_find(SmolRTSP_HeaderMap *restrict self, SmolRTSP_Slice key) {
    precondition(self);
    precondition(!SmolRTSP_Slice_is_null(key));

    for (size_t i = 0; i < self->len; i++) {
        if (SmolRTSP_Slice_eq(&self->headers[i].key, &key)) {
            return self->headers[i].value;
        }
    }

    return SmolRTSP_Slice_null();
}

void SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);
    precondition(keys_and_values_are_not_null_slices(self));

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
    precondition(keys_and_values_are_not_null_slices(self));

    for (size_t i = 0; i < self->len; i++) {
        SmolRTSP_Header_dbg_to_file(&self->headers[i], stream);
    }
}

void SmolRTSP_HeaderMap_dbg(const SmolRTSP_HeaderMap *self) {
    precondition(self);
    precondition(keys_and_values_are_not_null_slices(self));

    SmolRTSP_HeaderMap_dbg_to_file(self, stdout);
}
