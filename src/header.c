#include "correctness.h"
#include "match.h"
#include <smolrtsp/header.h>

#include <string.h>

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    SmolRTSP_Slice_serialize(&self->key, user_writer, user_cx);
    user_writer(strlen(": "), ": ", user_cx);
    SmolRTSP_Slice_serialize(&self->value, user_writer, user_cx);
}

bool SmolRTSP_Header_eq(const SmolRTSP_Header *restrict lhs, const SmolRTSP_Header *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return SmolRTSP_Slice_eq(&lhs->key, &rhs->key) && SmolRTSP_Slice_eq(&lhs->value, &rhs->value);
}

void SmolRTSP_Header_dbg_to_file(const SmolRTSP_Header *self, FILE *stream) {
    precondition(self);
    precondition(stream);

    fprintf(
        stream, "'%.*s': '%.*s'\n", (int)self->key.size, (const char *)self->key.ptr,
        (int)self->value.size, (const char *)self->value.ptr);
}

void SmolRTSP_Header_dbg(const SmolRTSP_Header *self) {
    precondition(self);

    SmolRTSP_Header_dbg_to_file(self, stdout);
}
