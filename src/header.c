#include "parsing_aux.h"
#include <smolrtsp/header.h>

#include <string.h>

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    static const char hcolon[] = ": ";

    user_writer(strlen(self->key), (const void *)self->key, user_cx);
    user_writer(strlen(hcolon), (const void *)hcolon, user_cx);
    user_writer(strlen(self->value), (const void *)self->value, user_cx);
}
