#include "../matching.h"
#include <smolrtsp/deserializers/crlf.h>
#include <smolrtsp/deserializers/method.h>
#include <smolrtsp/deserializers/request_line.h>
#include <smolrtsp/deserializers/request_uri.h>
#include <smolrtsp/deserializers/rtsp_version.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    SmolRTSP_MethodDeserializer *method;
    SmolRTSP_RequestURIDeserializer *uri;
    SmolRTSP_RTSPVersionDeserializer *version;
    SmolRTSP_CRLFDeserializer *crlf;
} InnerDeserializers;

typedef enum {
    InitDeserializersResultOk,
    InitDeserializersResultErr,
} InitDeserializersResult;

static InitDeserializersResult InnerDeserializers_init(InnerDeserializers *self) {
#define INIT(expr)                                                                                 \
    do {                                                                                           \
        if ((expr) == NULL) {                                                                      \
            return InitDeserializersResultErr;                                                     \
        }                                                                                          \
    } while (false)

    INIT(self->method = SmolRTSP_MethodDeserializer_new());
    INIT(self->uri = SmolRTSP_RequestURIDeserializer_new());
    INIT(self->version = SmolRTSP_RTSPVersionDeserializer_new());
    INIT(self->crlf = SmolRTSP_CRLFDeserializer_new());

#undef INIT

    return InitDeserializersResultOk;
}

static void InnerDeserializers_free(InnerDeserializers self) {
    SmolRTSP_MethodDeserializer_free(self.method);
    SmolRTSP_RequestURIDeserializer_free(self.uri);
    SmolRTSP_RTSPVersionDeserializer_free(self.version);
    SmolRTSP_CRLFDeserializer_free(self.crlf);
}

struct SmolRTSP_RequestLineDeserializer {
    SmolRTSP_RequestLineDeserializerState state;
    SmolRTSP_RequestLine inner;
    size_t bytes_read;
    InnerDeserializers inner_deserializers;
};

SmolRTSP_RequestLineDeserializer *SmolRTSP_RequestLineDeserializer_new(void) {
    SmolRTSP_RequestLineDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->state = SmolRTSP_RequestLineDeserializerStateStart;
    self->bytes_read = 0;
    if (InnerDeserializers_init(&self->inner_deserializers) == InitDeserializersResultErr) {
        free(self);
        return NULL;
    }

    return self;
}

void SmolRTSP_RequestLineDeserializer_free(SmolRTSP_RequestLineDeserializer *self) {
    InnerDeserializers_free(self->inner_deserializers);
    free(self);
}

SmolRTSP_RequestLineDeserializerState
SmolRTSP_RequestLineDeserializer_state(const SmolRTSP_RequestLineDeserializer *self) {
    assert(self);
    return self->state;
}

SmolRTSP_RequestLine
SmolRTSP_RequestLineDeserializer_inner(SmolRTSP_RequestLineDeserializer *self) {
    assert(self);
    return self->inner;
}

size_t SmolRTSP_RequestLineDeserializer_bytes_read(SmolRTSP_RequestLineDeserializer *self) {
    assert(self);
    return self->bytes_read;
}

#define ASSOC(current_state, deser_type, var, next_state, error)                                   \
    case current_state: {                                                                          \
        deser_type *deser = self->inner_deserializers.var;                                         \
        res = deser_type##_deserialize(deser, SmolRTSP_Slice_new(str + self->bytes_read, size));   \
        size_t bytes_read = deser_type##_bytes_read(deser);                                        \
                                                                                                   \
        switch (res) {                                                                             \
        case SmolRTSP_DeserializeResultOk:                                                         \
            self->bytes_read += bytes_read;                                                        \
            size -= bytes_read;                                                                    \
            self->state = next_state;                                                              \
            self->inner.var = deser_type##_inner(deser);                                           \
            break;                                                                                 \
        case SmolRTSP_DeserializeResultErr:                                                        \
            self->state = error;                                                                   \
            return res;                                                                            \
        case SmolRTSP_DeserializeResultNeedMore:                                                   \
            return res;                                                                            \
        }                                                                                          \
    } break

SmolRTSP_DeserializeResult SmolRTSP_RequestLineDeserializer_deserialize(
    SmolRTSP_RequestLineDeserializer *restrict self, SmolRTSP_Slice data) {
    assert(self);
    assert(!SmolRTSP_Slice_is_null(data));

    const char *str = data.ptr;
    size_t size = data.size;

    SmolRTSP_DeserializeResult res;
    while (true) {
        switch (self->state) {
            ASSOC(
                SmolRTSP_RequestLineDeserializerStateStart, SmolRTSP_MethodDeserializer, method,
                SmolRTSP_RequestLineDeserializerStateMethodParsed,
                SmolRTSP_RequestLineDeserializerStateMethodErr);
            ASSOC(
                SmolRTSP_RequestLineDeserializerStateMethodParsed, SmolRTSP_RequestURIDeserializer,
                uri, SmolRTSP_RequestLineDeserializerStateRequestURIParsed,
                SmolRTSP_RequestLineDeserializerStateRequestURIErr);
            ASSOC(
                SmolRTSP_RequestLineDeserializerStateRequestURIParsed,
                SmolRTSP_RTSPVersionDeserializer, version,
                SmolRTSP_RequestLineDeserializerStateRTSPVersionParsed,
                SmolRTSP_RequestLineDeserializerStateRTSPVersionErr);
            ASSOC(
                SmolRTSP_RequestLineDeserializerStateRTSPVersionParsed, SmolRTSP_CRLFDeserializer,
                crlf, SmolRTSP_RequestLineDeserializerStateCRLFParsed,
                SmolRTSP_RequestLineDeserializerStateCRLFErr);
        case SmolRTSP_RequestLineDeserializerStateCRLFParsed:
            return SmolRTSP_DeserializeResultOk;
        default:
            return SmolRTSP_DeserializeResultErr;
        }
    }
}

#undef ASSOC
