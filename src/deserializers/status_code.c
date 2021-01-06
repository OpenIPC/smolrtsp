#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/deserializers/status_code.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct SmolRTSP_StatusCodeDeserializer {
    size_t bytes_read;
};

SmolRTSP_StatusCodeDeserializer *SmolRTSP_StatusCodeDeserializer_new(void) {
    SmolRTSP_StatusCodeDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;
    return self;
}

void SmolRTSP_StatusCodeDeserializer_free(SmolRTSP_StatusCodeDeserializer *self) {
    free(self);
}

size_t SmolRTSP_StatusCodeDeserializer_bytes_read(SmolRTSP_StatusCodeDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_StatusCodeDeserializer_deserialize(
    SmolRTSP_StatusCodeDeserializer *restrict self, SmolRTSP_StatusCode *restrict result,
    Slice99 *restrict data) {
    precondition(self);
    precondition(result);
    precondition(data);

    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    const char *code = data->ptr;
    MATCH(SmolRTSP_match_numeric(data, &bytes_read));
    const size_t code_size = (const char *)data->ptr - code;

    SmolRTSP_StatusCode code_int;
    char format[50];
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, code_size);
    int rc = sscanf(code, format, &code_int);
    precondition(rc == 1);

    self->bytes_read = bytes_read;
    *result = code_int;

    return SmolRTSP_DeserializeResultOk;
}
