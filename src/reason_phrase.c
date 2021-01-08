#include "correctness.h"
#include "match.h"
#include <smolrtsp/reason_phrase.h>

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhrase_deserialize(
    SmolRTSP_ReasonPhrase *restrict self, Slice99 *restrict data, size_t *restrict bytes_read) {
    precondition(self);
    precondition(data);
    precondition(bytes_read);

    size_t bytes_read_temp = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read_temp));
    self->ptr = data->ptr;
    MATCH(SmolRTSP_match_until_crlf(data, &bytes_read_temp));
    *self = Slice99_from_ptrdiff(self->ptr, data->ptr - strlen("\r\n"), sizeof(char));

    *bytes_read = bytes_read_temp;

    return SmolRTSP_DeserializeResultOk;
}
