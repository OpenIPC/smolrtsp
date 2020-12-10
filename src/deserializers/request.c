#include "../deser_aux.h"
#include <smolrtsp/deserializers/header_map.h>
#include <smolrtsp/deserializers/message_body.h>
#include <smolrtsp/deserializers/request.h>
#include <smolrtsp/deserializers/request_line.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    SmolRTSP_RequestLineDeserializer *start_line;
    SmolRTSP_HeaderMapDeserializer *header_map;
    SmolRTSP_MessageBodyDeserializer *body;
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

    INIT(self->start_line = SmolRTSP_RequestLineDeserializer_new());
    INIT(self->header_map = SmolRTSP_HeaderMapDeserializer_new());
    self->body = NULL;

#undef INIT
}

static void InnerDeserializers_free(InnerDeserializers self) {
    SmolRTSP_RequestLineDeserializer_free(self.start_line);
    SmolRTSP_HeaderMapDeserializer_free(self.header_map);
    SmolRTSP_MessageBodyDeserializer_free(self.body);
}

struct SmolRTSP_RequestDeserializer {
    SmolRTSP_RequestDeserializerState state;
    SmolRTSP_Request inner;
    size_t bytes_read;
    InnerDeserializers inner_deserializers;
};

SmolRTSP_RequestDeserializer *SmolRTSP_RequestDeserializer_new(void) {
    SmolRTSP_RequestDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->state = SmolRTSP_RequestDeserializerStateStart;
    self->bytes_read = 0;
    if (InnerDeserializers_init(&self->inner_deserializers) == InitDeserializersResultErr) {
        free(self);
        return NULL;
    }

    return self;
}

void SmolRTSP_RequestDeserializer_free(SmolRTSP_RequestDeserializer *self) {
    InnerDeserializers_free(self->inner_deserializers);
    free(self);
}

SmolRTSP_RequestDeserializerState
SmolRTSP_RequestDeserializer_state(const SmolRTSP_RequestDeserializer *self) {
    return self->state;
}

SmolRTSP_Request SmolRTSP_RequestDeserializer_inner(SmolRTSP_RequestDeserializer *self) {
    assert(self);
    return self->inner;
}

size_t SmolRTSP_RequestDeserializer_bytes_read(SmolRTSP_RequestDeserializer *self) {
    assert(self);
    return self->bytes_read;
}

typedef enum {
    InitBodyDeserializerResultInvalidContentLength,
    InitBodyDeserializerResultOk,
} InitBodyDeserializerResult;

static InitBodyDeserializerResult init_body_deserializer(SmolRTSP_RequestDeserializer *self);

#define ASSOC(current_state, deser_type, var, next_state, error)                                   \
    case current_state: {                                                                          \
        deser_type *deser = self->inner_deserializers.var;                                         \
        res = deser_type##_deserialize(deser, size, data + self->bytes_read);                      \
        size_t bytes_read = deser_type##_bytes_read(deser);                                        \
                                                                                                   \
        switch (res) {                                                                             \
        case SmolRTSP_DeserializeResultOk:                                                         \
            self->bytes_read += bytes_read;                                                        \
            size -= bytes_read;                                                                    \
            self->state = next_state;                                                              \
            self->inner.var = deser_type##_inner(deser);                                           \
                                                                                                   \
            if (self->state == SmolRTSP_RequestDeserializerStateHeaderMapParsed) {                 \
                if (init_body_deserializer(self) ==                                                \
                    InitBodyDeserializerResultInvalidContentLength) {                              \
                    return SmolRTSP_DeserializeResultErr;                                          \
                }                                                                                  \
            }                                                                                      \
            break;                                                                                 \
        case SmolRTSP_DeserializeResultErr:                                                        \
            self->state = error;                                                                   \
            return res;                                                                            \
        case SmolRTSP_DeserializeResultNeedMore:                                                   \
            return res;                                                                            \
        }                                                                                          \
    } break

SmolRTSP_DeserializeResult SmolRTSP_RequestDeserializer_deserialize(
    SmolRTSP_RequestDeserializer *restrict self, size_t size,
    const char data[restrict static size]) {
    assert(self);
    assert(data);

    SmolRTSP_DeserializeResult res;
    while (true) {
        switch (self->state) {
            ASSOC(
                SmolRTSP_RequestDeserializerStateStart, SmolRTSP_RequestLineDeserializer,
                start_line, SmolRTSP_RequestDeserializerStateRequestLineParsed,
                SmolRTSP_RequestDeserializerStateRequestLineErr);
            ASSOC(
                SmolRTSP_RequestDeserializerStateRequestLineParsed, SmolRTSP_HeaderMapDeserializer,
                header_map, SmolRTSP_RequestDeserializerStateHeaderMapParsed,
                SmolRTSP_RequestDeserializerStateHeaderMapErr);
            ASSOC(
                SmolRTSP_RequestDeserializerStateHeaderMapParsed, SmolRTSP_MessageBodyDeserializer,
                body, SmolRTSP_RequestDeserializerStateMessageBodyParsed,
                SmolRTSP_RequestDeserializerStateMessageBodyErr);
        case SmolRTSP_RequestDeserializerStateMessageBodyParsed:
            return SmolRTSP_DeserializeResultOk;
        default:
            return SmolRTSP_DeserializeResultErr;
        }
    }
}

#undef ASSOC

static InitBodyDeserializerResult init_body_deserializer(SmolRTSP_RequestDeserializer *self) {
    const char *content_length_str =
        SmolRTSP_HeaderMap_find(&self->inner.header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH);

    size_t content_length;

    if (content_length_str == NULL) {
        content_length = 0;
    } else if (sscanf(content_length_str, "%zd", &content_length) != 1) {
        self->state = SmolRTSP_RequestDeserializerStateContentLengthErr;
        return InitBodyDeserializerResultInvalidContentLength;
    }

    self->inner_deserializers.body = SmolRTSP_MessageBodyDeserializer_new(content_length);
    return InitBodyDeserializerResultOk;
}
