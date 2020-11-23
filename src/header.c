#include "parsing_aux.h"
#include <smolrtsp/header.h>

#include <string.h>

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header header, SmolRTSP_UserWriter user_writer, void *user_cx) {
    static const char hcolon[] = ": ";

    user_writer(strlen(header.key), (const void *)header.key, user_cx);
    user_writer(strlen(hcolon), (const void *)hcolon, user_cx);
    user_writer(strlen(header.value), (const void *)header.value, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_Header_deserialize(
    SmolRTSP_Header *restrict header, size_t size, const void *restrict data,
    size_t *restrict bytes_read) {}
