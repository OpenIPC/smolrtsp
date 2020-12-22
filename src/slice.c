#include "aux_macros.h"
#include "correctness.h"
#include <smolrtsp/slice.h>

#include <string.h>

SmolRTSP_Slice SmolRTSP_Slice_new(const void *data, size_t size) {
    return (SmolRTSP_Slice){.ptr = data, .size = size};
}

SmolRTSP_Slice SmolRTSP_Slice_null(void) {
    return SmolRTSP_Slice_new(NULL, 0);
}

bool SmolRTSP_Slice_is_null(SmolRTSP_Slice self) {
    return self.ptr == NULL;
}

bool SmolRTSP_Slice_is_empty(SmolRTSP_Slice self) {
    return self.size == 0;
}

SmolRTSP_Slice SmolRTSP_Slice_advance(SmolRTSP_Slice self, size_t offset) {
    precondition(!SmolRTSP_Slice_is_null(self));
    precondition(offset <= self.size);

    return SmolRTSP_Slice_new(self.ptr + offset, self.size - offset);
}

SmolRTSP_Slice SmolRTSP_Slice_go_back(SmolRTSP_Slice self, size_t offset) {
    precondition(!SmolRTSP_Slice_is_null(self));
    precondition(offset <= self.size);

    return SmolRTSP_Slice_new(self.ptr - offset, self.size + offset);
}

SmolRTSP_Slice SmolRTSP_Slice_from_str(const char *str) {
    if (str == NULL) {
        return SmolRTSP_Slice_null();
    }

    return SmolRTSP_Slice_new(str, strlen(str));
}

bool SmolRTSP_Slice_eq(const SmolRTSP_Slice *restrict lhs, const SmolRTSP_Slice *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    if (SmolRTSP_Slice_is_null(*lhs) && SmolRTSP_Slice_is_null(*rhs)) {
        return true;
    }

    if (lhs->size != rhs->size) {
        printf("%zd|%zd\n", lhs->size, rhs->size);
        return false;
    }

    const size_t size = lhs->size;
    return memcmp(lhs->ptr, rhs->ptr, size) == 0;
}

void SmolRTSP_Slice_serialize(
    const SmolRTSP_Slice *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);
    precondition(!SmolRTSP_Slice_is_null(*self));

    user_writer(self->size, self->ptr, user_cx);
}

void SmolRTSP_Slice_print_s_to_file(SmolRTSP_Slice data, FILE *stream) {
    precondition(!SmolRTSP_Slice_is_null(data));

    fwrite(data.ptr, 1, data.size, stream);
}

void SmolRTSP_Slice_print_s(SmolRTSP_Slice data) {
    precondition(!SmolRTSP_Slice_is_null(data));

    SmolRTSP_Slice_print_s_to_file(data, stdout);
}

void SmolRTSP_Slice_print_s_to_file_ln(SmolRTSP_Slice data, FILE *stream) {
    precondition(!SmolRTSP_Slice_is_null(data));

    SmolRTSP_Slice_print_s_to_file(data, stream);
    fprintf(stream, "\n");
}

void SmolRTSP_Slice_print_s_ln(SmolRTSP_Slice data) {
    precondition(!SmolRTSP_Slice_is_null(data));

    SmolRTSP_Slice_print_s(data);
    puts("");
}

SmolRTSP_MutSlice SmolRTSP_MutSlice_new(void *data, size_t size) {
    return (SmolRTSP_MutSlice){.ptr = data, .size = size};
}

SmolRTSP_MutSlice SmolRTSP_MutSlice_null(void) {
    return SmolRTSP_MutSlice_new(NULL, 0);
}

bool SmolRTSP_MutSlice_is_null(SmolRTSP_MutSlice self) {
    return SmolRTSP_Slice_is_null(SmolRTSP_MutSlice_to_const(self));
}

bool SmolRTSP_MutSlice_is_empty(SmolRTSP_MutSlice self) {
    return SmolRTSP_Slice_is_empty(SmolRTSP_MutSlice_to_const(self));
}

SmolRTSP_MutSlice SmolRTSP_MutSlice_advance(SmolRTSP_MutSlice self, size_t offset) {
    precondition(!SmolRTSP_MutSlice_is_null(self));
    precondition(offset <= self.size);

    return SmolRTSP_MutSlice_new(self.ptr + offset, self.size - offset);
}

SmolRTSP_MutSlice SmolRTSP_MutSlice_from_str(char *str) {
    if (str == NULL) {
        return SmolRTSP_MutSlice_null();
    }

    return SmolRTSP_MutSlice_new(str, strlen(str));
}

bool SmolRTSP_MutSlice_eq(
    const SmolRTSP_MutSlice *restrict lhs, const SmolRTSP_MutSlice *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return SmolRTSP_Slice_eq(
        OBJ(SmolRTSP_Slice, SmolRTSP_MutSlice_to_const(*lhs)),
        OBJ(SmolRTSP_Slice, SmolRTSP_MutSlice_to_const(*rhs)));
}

void SmolRTSP_MutSlice_serialize(
    const SmolRTSP_MutSlice *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    SmolRTSP_Slice_serialize(
        OBJ(SmolRTSP_Slice, SmolRTSP_MutSlice_to_const(*self)), user_writer, user_cx);
}

SmolRTSP_Slice SmolRTSP_MutSlice_to_const(SmolRTSP_MutSlice self) {
    return SmolRTSP_Slice_new(self.ptr, self.size);
}
