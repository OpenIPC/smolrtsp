#include "../parsing_aux.h"
#include <smolrtsp/deserializers/method.h>
#include <smolrtsp/deserializers/request_line.h>
#include <smolrtsp/deserializers/request_uri.h>
#include <smolrtsp/deserializers/rtsp_version.h>

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    SmolRTSP_MethodDeserializer *method;
    SmolRTSP_RequestURIDeserializer *uri;
    SmolRTSP_RTSPVersionDeserializer *version;
} InnerDeserializers;

static void InnerDeserializers_init(InnerDeserializers *self) {
    self->method = SmolRTSP_MethodDeserializer_new();
    self->uri = SmolRTSP_RequestURIDeserializer_new();
    self->version = SmolRTSP_RTSPVersionDeserializer_new();
}

static void InnerDeserializers_free(InnerDeserializers self) {
    SmolRTSP_MethodDeserializer_free(self.method);
    SmolRTSP_RequestURIDeserializer_free(self.uri);
    SmolRTSP_RTSPVersionDeserializer_free(self.version);
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
    InnerDeserializers_init(&self->inner_deserializers);

    return self;
}

void SmolRTSP_RequestLineDeserializer_free(SmolRTSP_RequestLineDeserializer *self) {
    InnerDeserializers_free(self->inner_deserializers);
    free(self);
}

SmolRTSP_RequestLineDeserializerState
SmolRTSP_RequestLineDeserializer_state(const SmolRTSP_RequestLineDeserializer *self) {
    return self->state;
}

SmolRTSP_RequestLine
SmolRTSP_RequestLineDeserializer_inner(SmolRTSP_RequestLineDeserializer *self) {
    return self->inner;
}

size_t SmolRTSP_RequestLineDeserializer_bytes_read(SmolRTSP_RequestLineDeserializer *self) {
    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RequestLineDeserializer_deserialize(
    SmolRTSP_RequestLineDeserializer *restrict self, size_t size, const void *restrict data) {
    SmolRTSP_DeserializeResult res;

#define ASSOC(current_state, next_type, var)                                                       \
    case SmolRTSP_RequestLineDeserializerState##current_state: {                                   \
        SmolRTSP_##next_type##Deserializer *deserializer = self->inner_deserializers.var;          \
        res = SmolRTSP_##next_type##Deserializer_deserialize(                                      \
            deserializer, size, data + self->bytes_read);                                          \
        size_t bytes_read = SmolRTSP_##next_type##Deserializer_bytes_read(deserializer);           \
                                                                                                   \
        switch (res) {                                                                             \
        case SmolRTSP_DeserializeResultOk:                                                         \
            self->bytes_read += bytes_read;                                                        \
            size -= bytes_read;                                                                    \
            self->state = SmolRTSP_RequestLineDeserializerState##next_type##Parsed;                \
            self->inner.var = SmolRTSP_##next_type##Deserializer_inner(deserializer);              \
            break;                                                                                 \
        case SmolRTSP_DeserializeResultErr:                                                        \
            self->state = SmolRTSP_RequestLineDeserializerStateErr;                                \
            return res;                                                                            \
        case SmolRTSP_DeserializeResultNeedMore:                                                   \
            return res;                                                                            \
        }                                                                                          \
    } break

    while (true) {
        switch (self->state) {
            ASSOC(Start, Method, method);
            ASSOC(MethodParsed, RequestURI, uri);
            ASSOC(RequestURIParsed, RTSPVersion, version);
        case SmolRTSP_RequestLineDeserializerStateRTSPVersionParsed:
            return SmolRTSP_DeserializeResultOk;
        }
    }

#undef ASSOC
}
