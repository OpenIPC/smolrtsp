#include "correctness.h"
#include "match.h"
#include <smolrtsp/request_uri.h>

SmolRTSP_DeserializeResult SmolRTSP_RequestURI_deserialize(
    SmolRTSP_RequestURI *restrict self, Slice99 *restrict data, size_t *restrict bytes_read) {
    precondition(self);
    precondition(data);
    precondition(bytes_read);

    size_t bytes_read_temp = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read_temp));
    self->ptr = data->ptr;
    MATCH(SmolRTSP_match_non_whitespaces(data, &bytes_read_temp));
    *self = Slice99_from_ptrdiff(self->ptr, data->ptr, sizeof(char));

    *bytes_read += bytes_read_temp;

    return SmolRTSP_DeserializeResultOk;
}
