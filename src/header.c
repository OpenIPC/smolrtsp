#include "matching.h"
#include <smolrtsp/header.h>

#include <assert.h>
#include <string.h>

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    SmolRTSP_Slice_serialize(&self->key, user_writer, user_cx);
    user_writer(strlen(": "), ": ", user_cx);
    SmolRTSP_Slice_serialize(&self->value, user_writer, user_cx);
}

bool SmolRTSP_Header_eq(const SmolRTSP_Header *restrict lhs, const SmolRTSP_Header *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return SmolRTSP_Slice_eq(&lhs->key, &rhs->key) && SmolRTSP_Slice_eq(&lhs->value, &rhs->value);
}

void SmolRTSP_Header_pretty_print_to_file(const SmolRTSP_Header *self, FILE *stream) {
    assert(self);
    assert(stream);

    fprintf(
        stream, "%.*s: %.*s\n", (int)self->key.size, (const char *)self->key.ptr,
        (int)self->value.size, (const char *)self->value.ptr);
}

void SmolRTSP_Header_pretty_print(const SmolRTSP_Header *self) {
    assert(self);

    SmolRTSP_Header_pretty_print_to_file(self, stdout);
}
