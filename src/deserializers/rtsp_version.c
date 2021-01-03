#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/deserializers/rtsp_version.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct SmolRTSP_RTSPVersionDeserializer {
    SmolRTSP_RTSPVersion inner;
    size_t bytes_read;
};

SmolRTSP_RTSPVersionDeserializer *SmolRTSP_RTSPVersionDeserializer_new(void) {
    SmolRTSP_RTSPVersionDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->bytes_read = 0;

    return self;
}

void SmolRTSP_RTSPVersionDeserializer_free(SmolRTSP_RTSPVersionDeserializer *self) {
    free(self);
}

SmolRTSP_RTSPVersion
SmolRTSP_RTSPVersionDeserializer_inner(SmolRTSP_RTSPVersionDeserializer *self) {
    precondition(self);

    return self->inner;
}

size_t SmolRTSP_RTSPVersionDeserializer_bytes_read(SmolRTSP_RTSPVersionDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RTSPVersionDeserializer_deserialize(
    SmolRTSP_RTSPVersionDeserializer *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    size_t bytes_read = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read));
    MATCH(SmolRTSP_match_str(data, &bytes_read, "RTSP/"));

    const char *major = data->ptr;
    MATCH(SmolRTSP_match_numeric(data, &bytes_read));
    const size_t major_size = (const char *)data->ptr - major;
    MATCH(SmolRTSP_match_char(data, &bytes_read, '.'));
    const char *minor = data->ptr;
    MATCH(SmolRTSP_match_numeric(data, &bytes_read));
    const size_t minor_size = (const char *)data->ptr - minor;

    uint_least8_t major_int;
    char format[50];
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, major_size);
    int rc = sscanf(major, format, &major_int);
    precondition(rc == 1);

    uint_least8_t minor_int;
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, minor_size);
    rc = sscanf(minor, format, &minor_int);
    precondition(rc == 1);

    self->bytes_read = bytes_read;
    self->inner.major = major_int;
    self->inner.minor = minor_int;

    return SmolRTSP_DeserializeResultOk;
}
