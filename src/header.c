#include <smolrtsp/header.h>

#include "common.h"
#include "parsing.h"

#include <assert.h>
#include <string.h>

void SmolRTSP_Header_serialize(
    SmolRTSP_Header self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(user_writer);

    user_writer(self.key, user_cx);
    user_writer(CharSlice99_from_str(": "), user_cx);
    user_writer(self.value, user_cx);
    user_writer(SMOLRTSP_CRLF, user_cx);
}

SmolRTSP_DeserializeResult
SmolRTSP_Header_deserialize(SmolRTSP_Header *restrict self, CharSlice99 *restrict data) {
    assert(self);
    assert(data);

    SmolRTSP_Header header;

    MATCH(SmolRTSP_match_whitespaces(data));
    header.key = *data;
    MATCH(SmolRTSP_match_header_name(data));
    header.key = CharSlice99_from_ptrdiff(header.key.ptr, data->ptr);

    MATCH(SmolRTSP_match_whitespaces(data));
    MATCH(SmolRTSP_match_char(data, ':'));
    MATCH(SmolRTSP_match_whitespaces(data));

    header.value = *data;
    MATCH(SmolRTSP_match_until_crlf(data));
    header.value = CharSlice99_from_ptrdiff(header.value.ptr, data->ptr - strlen("\r\n"));

    *self = header;

    return SmolRTSP_DeserializeResult_Ok;
}

bool SmolRTSP_Header_eq(SmolRTSP_Header lhs, SmolRTSP_Header rhs) {
    return CharSlice99_primitive_eq(lhs.key, rhs.key) &&
           CharSlice99_primitive_eq(lhs.value, rhs.value);
}

void SmolRTSP_Header_dbg_to_file(SmolRTSP_Header self, FILE *stream) {
    assert(stream);

    fprintf(
        stream, "'%.*s': '%.*s'\n", (int)CharSlice99_size(self.key), (const char *)self.key.ptr,
        (int)CharSlice99_size(self.value), (const char *)self.value.ptr);
}

void SmolRTSP_Header_dbg(SmolRTSP_Header self) {
    SmolRTSP_Header_dbg_to_file(self, stderr);
}
