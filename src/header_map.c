#include "deser_aux.h"
#include <smolrtsp/header_map.h>

#include <assert.h>
#include <string.h>

SmolRTSP_Slice
SmolRTSP_HeaderMap_find(SmolRTSP_HeaderMap *restrict self, const char *restrict key) {
    assert(self);
    assert(key);

    const SmolRTSP_Slice key_slice = {.data = key, .size = strlen(key)};

    for (size_t i = 0; i < self->count; i++) {
        if (SmolRTSP_Slice_eq(&self->headers[i].key, &key_slice)) {
            return self->headers[i].value;
        }
    }

    return SmolRTSP_Slice_null();
}

void SmolRTSP_HeaderMap_serialize(
    const SmolRTSP_HeaderMap *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    for (size_t i = 0; i < self->count; i++) {
        SmolRTSP_Header_serialize(&self->headers[i], user_writer, user_cx);
    }

    user_writer(strlen(CRLF), CRLF, user_cx);
}

bool SmolRTSP_HeaderMap_eq(const SmolRTSP_HeaderMap *lhs, const SmolRTSP_HeaderMap *rhs) {
    assert(lhs);
    assert(rhs);

    if (lhs->count != rhs->count) {
        return false;
    }

    const size_t count = lhs->count;

    for (size_t i = 0; i < count; i++) {
        if (!SmolRTSP_Header_eq(&lhs->headers[i], &rhs->headers[i])) {
            return false;
        }
    }

    return true;
}
