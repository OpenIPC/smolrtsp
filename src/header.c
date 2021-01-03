#include "correctness.h"
#include "match.h"
#include <smolrtsp/header.h>

#include <string.h>

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    user_writer(Slice99_size(self->key), self->key.ptr, user_cx);
    user_writer(strlen(": "), ": ", user_cx);
    user_writer(Slice99_size(self->value), self->value.ptr, user_cx);
}

bool SmolRTSP_Header_eq(const SmolRTSP_Header *restrict lhs, const SmolRTSP_Header *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return Slice99_primitive_eq(lhs->key, rhs->key) && Slice99_primitive_eq(lhs->value, rhs->value);
}

void SmolRTSP_Header_dbg_to_file(const SmolRTSP_Header *self, FILE *stream) {
    precondition(self);
    precondition(stream);

    fprintf(
        stream, "'%.*s': '%.*s'\n", (int)Slice99_size(self->key), (const char *)self->key.ptr,
        (int)Slice99_size(self->value), (const char *)self->value.ptr);
}

void SmolRTSP_Header_dbg(const SmolRTSP_Header *self) {
    precondition(self);

    SmolRTSP_Header_dbg_to_file(self, stdout);
}
