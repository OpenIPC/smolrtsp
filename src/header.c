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

SmolRTSP_DeserializeResult SmolRTSP_Header_deserialize(
    SmolRTSP_Header *restrict self, Slice99 *restrict data, size_t *restrict bytes_read) {
    precondition(self);
    precondition(data);
    precondition(bytes_read);

    SmolRTSP_Header header;
    size_t bytes_read_temp = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read_temp));
    header.key = *data;
    MATCH(SmolRTSP_match_header_name(data, &bytes_read_temp));
    header.key = Slice99_from_ptrdiff(header.key.ptr, data->ptr, sizeof(char));

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read_temp));
    MATCH(SmolRTSP_match_char(data, &bytes_read_temp, ':'));
    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read_temp));

    header.value = *data;

    MATCH(SmolRTSP_match_until_crlf(data, &bytes_read_temp));
    header.value = Slice99_from_ptrdiff(header.value.ptr, data->ptr - strlen("\r\n"), sizeof(char));

    *self = header;
    *bytes_read += bytes_read_temp;

    return SmolRTSP_DeserializeResultOk;
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
