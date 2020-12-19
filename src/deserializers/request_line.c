#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/deserializers/method.h>
#include <smolrtsp/deserializers/request_line.h>
#include <smolrtsp/deserializers/request_uri.h>
#include <smolrtsp/deserializers/rtsp_version.h>

#include <stdbool.h>
#include <stdlib.h>

struct SmolRTSP_RequestLineDeserializer {
    SmolRTSP_RequestLineDeserializerState state;
    SmolRTSP_RequestLine inner;
    size_t bytes_read;

    struct {
        SmolRTSP_MethodDeserializer *method;
        SmolRTSP_RequestURIDeserializer *uri;
        SmolRTSP_RTSPVersionDeserializer *version;
    } deserializers;
};

SmolRTSP_RequestLineDeserializer *SmolRTSP_RequestLineDeserializer_new(void) {
    SmolRTSP_RequestLineDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->state = (SmolRTSP_RequestLineDeserializerState){
        .in_progress = SmolRTSP_RequestLineDeserializerStateInProgressMethod,
        .is_ok = true,
    };
    self->bytes_read = 0;

#define INIT(expr)                                                                                 \
    do {                                                                                           \
        if ((expr) == NULL) {                                                                      \
            free(self);                                                                            \
            return NULL;                                                                           \
        }                                                                                          \
    } while (false)

    INIT(self->deserializers.method = SmolRTSP_MethodDeserializer_new());
    INIT(self->deserializers.uri = SmolRTSP_RequestURIDeserializer_new());
    INIT(self->deserializers.version = SmolRTSP_RTSPVersionDeserializer_new());

#undef INIT

    return self;
}

void SmolRTSP_RequestLineDeserializer_free(SmolRTSP_RequestLineDeserializer *self) {
    precondition(self);

    SmolRTSP_MethodDeserializer_free(self->deserializers.method);
    SmolRTSP_RequestURIDeserializer_free(self->deserializers.uri);
    SmolRTSP_RTSPVersionDeserializer_free(self->deserializers.version);
    free(self);
}

SmolRTSP_RequestLineDeserializerState
SmolRTSP_RequestLineDeserializer_state(const SmolRTSP_RequestLineDeserializer *self) {
    precondition(self);

    return self->state;
}

SmolRTSP_RequestLine
SmolRTSP_RequestLineDeserializer_inner(SmolRTSP_RequestLineDeserializer *self) {
    precondition(self);

    return self->inner;
}

size_t SmolRTSP_RequestLineDeserializer_bytes_read(SmolRTSP_RequestLineDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RequestLineDeserializer_deserialize(
    SmolRTSP_RequestLineDeserializer *restrict self, SmolRTSP_Slice *restrict data) {
    precondition(self);
    precondition(data);
    precondition(!SmolRTSP_Slice_is_null(*data));

    // TODO: Make an eDSL for this shit.
    if (self->state.in_progress == SmolRTSP_RequestLineDeserializerStateInProgressMethod) {
        MATCH(SmolRTSP_MethodDeserializer_deserialize(self->deserializers.method, data));
        self->inner.method = SmolRTSP_MethodDeserializer_inner(self->deserializers.method);
        self->bytes_read += SmolRTSP_MethodDeserializer_bytes_read(self->deserializers.method);
        self->state.in_progress = SmolRTSP_RequestLineDeserializerStateInProgressRequestURI;
    }

    if (self->state.in_progress == SmolRTSP_RequestLineDeserializerStateInProgressRequestURI) {
        MATCH(SmolRTSP_RequestURIDeserializer_deserialize(self->deserializers.uri, data));
        self->inner.uri = SmolRTSP_RequestURIDeserializer_inner(self->deserializers.uri);
        self->bytes_read += SmolRTSP_RequestURIDeserializer_bytes_read(self->deserializers.uri);
        self->state.in_progress = SmolRTSP_RequestLineDeserializerStateInProgressRTSPVersion;
    }

    if (self->state.in_progress == SmolRTSP_RequestLineDeserializerStateInProgressRTSPVersion) {
        MATCH(SmolRTSP_RTSPVersionDeserializer_deserialize(self->deserializers.version, data));
        self->inner.version = SmolRTSP_RTSPVersionDeserializer_inner(self->deserializers.version);
        self->bytes_read +=
            SmolRTSP_RTSPVersionDeserializer_bytes_read(self->deserializers.version);
        self->state.in_progress = SmolRTSP_RequestLineDeserializerStateInProgressCRLF;
    }

    if (self->state.in_progress == SmolRTSP_RequestLineDeserializerStateInProgressCRLF) {
        MATCH(SmolRTSP_match_str(data, &self->bytes_read, SMOLRTSP_CRLF));
        self->state.in_progress = SmolRTSP_RequestLineDeserializerStateInProgressDone;
    }

    return SmolRTSP_DeserializeResultOk;
}
