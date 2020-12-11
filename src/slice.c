#include "aux.h"
#include <smolrtsp/slice.h>

#include <assert.h>
#include <string.h>

SmolRTSP_Slice SmolRTSP_Slice_new(const void *data, size_t size) {
    return (SmolRTSP_Slice){.data = data, .size = size};
}

SmolRTSP_Slice SmolRTSP_Slice_null(void) {
    return (SmolRTSP_Slice){.data = NULL, .size = 0};
}

bool SmolRTSP_Slice_is_null(SmolRTSP_Slice self) {
    return self.data == NULL;
}

SmolRTSP_Slice SmolRTSP_Slice_from_str(const char *str) {
    if (str == NULL) {
        return SmolRTSP_Slice_null();
    }

    return (SmolRTSP_Slice){.data = str, .size = strlen(str)};
}

bool SmolRTSP_Slice_eq(const SmolRTSP_Slice *lhs, const SmolRTSP_Slice *rhs) {
    assert(lhs);
    assert(rhs);

    if (lhs->size != rhs->size) {
        return false;
    }

    const size_t size = lhs->size;
    return memcmp(lhs->data, rhs->data, size) == 0;
}

void SmolRTSP_Slice_serialize(
    const SmolRTSP_Slice *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);
    user_writer(self->size, self->data, user_cx);
}

SmolRTSP_MutSlice SmolRTSP_MutSlice_null(void) {
    return (SmolRTSP_MutSlice){.data = NULL, .size = 0};
}

bool SmolRTSP_MutSlice_is_null(SmolRTSP_MutSlice self) {
    return SmolRTSP_Slice_is_null(SmolRTSP_MutSlice_to_const(self));
}

SmolRTSP_MutSlice SmolRTSP_MutSlice_from_str(char *str) {
    if (str == NULL) {
        return SmolRTSP_MutSlice_null();
    }

    return (SmolRTSP_MutSlice){.data = str, .size = strlen(str)};
}

bool SmolRTSP_MutSlice_eq(const SmolRTSP_MutSlice *lhs, const SmolRTSP_MutSlice *rhs) {
    assert(lhs);
    assert(rhs);

    return SmolRTSP_Slice_eq(
        OBJ(SmolRTSP_Slice, SmolRTSP_MutSlice_to_const(*lhs)),
        OBJ(SmolRTSP_Slice, SmolRTSP_MutSlice_to_const(*rhs)));
}

void SmolRTSP_MutSlice_serialize(
    const SmolRTSP_MutSlice *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);
    SmolRTSP_Slice_serialize(
        OBJ(SmolRTSP_Slice, SmolRTSP_MutSlice_to_const(*self)), user_writer, user_cx);
}

SmolRTSP_Slice SmolRTSP_MutSlice_to_const(SmolRTSP_MutSlice self) {
    return (SmolRTSP_Slice){.data = self.data, .size = self.size};
}
