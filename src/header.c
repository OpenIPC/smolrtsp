#include "deser_aux.h"
#include <smolrtsp/header.h>

#include <string.h>

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    static const char hcolon[] = ": ";

    user_writer(self->key_len, self->key, user_cx);
    user_writer(strlen(hcolon), hcolon, user_cx);
    user_writer(self->value_len, self->value, user_cx);
}

bool SmolRTSP_Header_eq(const SmolRTSP_Header *lhs, const SmolRTSP_Header *rhs) {
    if (lhs->key_len != rhs->key_len || lhs->value_len != rhs->value_len) {
        return false;
    }

    size_t key_len = lhs->key_len, value_len = lhs->value_len;

    return strncmp(lhs->key, rhs->key, key_len) == 0 &&
           strncmp(lhs->value, rhs->value, value_len) == 0;
}
