#include "../parsing_aux.h"
#include <smolrtsp/deserializers/header_map.h>
#include <smolrtsp/deserializers/message_body.h>
#include <smolrtsp/deserializers/request.h>
#include <smolrtsp/deserializers/request_line.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    SmolRTSP_RequestLineDeserializer *start_line;
    SmolRTSP_HeaderMapDeserializer *header_map;
    SmolRTSP_MessageBodyDeserializer *body;
} InnerDeserializers;

static void InnerDeserializers_init(InnerDeserializers *self) {
    self->start_line = SmolRTSP_RequestLineDeserializer_new();
    self->header_map = SmolRTSP_HeaderMapDeserializer_new();
    self->body = NULL;
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
    InnerDeserializers_init(&self->inner_deserializers);

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
    return self->inner;
}

size_t SmolRTSP_RequestDeserializer_bytes_read(SmolRTSP_RequestDeserializer *self) {
    return self->bytes_read;
}

typedef SmolRTSP_DeserializeResult (*Deserializer)(
    void *restrict self, void *restrict entity, size_t size, const void *restrict data,
    size_t *restrict bytes_read);

SmolRTSP_DeserializeResult SmolRTSP_RequestDeserializer_deserialize(
    SmolRTSP_RequestDeserializer *restrict self, size_t size, const void *restrict data) {
    if (self->state == SmolRTSP_RequestDeserializerStateMessageBodyParsed ||
        self->state == SmolRTSP_RequestDeserializerStateErr) {
        return self->state;
    }

#define ASSOC(current_state, next_type, var)                                                       \
    case SmolRTSP_RequestDeserializerState##current_state: {                                       \
        SmolRTSP_##next_type##Deserializer *deserializer = self->inner_deserializers.var;          \
        res = SmolRTSP_##next_type##Deserializer_deserialize(deserializer, size, data);            \
        size_t bytes_read = SmolRTSP_##next_type##Deserializer_bytes_read(deserializer);           \
                                                                                                   \
        switch (res) {                                                                             \
        case SmolRTSP_DeserializeResultOk:                                                         \
            self->bytes_read += bytes_read;                                                        \
            size -= bytes_read;                                                                    \
            self->state = SmolRTSP_RequestDeserializerState##next_type##Parsed;                    \
            self->inner.var = SmolRTSP_##next_type##Deserializer_inner(deserializer);              \
                                                                                                   \
            if (self->state == SmolRTSP_RequestDeserializerStateHeaderMapParsed) {                 \
                const char *content_length_value;                                                  \
                if ((content_length_value = SmolRTSP_HeaderMap_find(                               \
                         &self->inner.header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH)) == NULL) { \
                    self->inner.body.data = NULL;                                                  \
                    self->state = SmolRTSP_RequestDeserializerStateMessageBodyParsed;              \
                    return self->state;                                                            \
                }                                                                                  \
                                                                                                   \
                size_t content_length;                                                             \
                if (sscanf(content_length_value, "%zd", &content_length) != 1) {                   \
                    self->state = SmolRTSP_RequestDeserializerStateErr;                            \
                    return self->state;                                                            \
                }                                                                                  \
                                                                                                   \
                self->inner_deserializers.body =                                                   \
                    SmolRTSP_MessageBodyDeserializer_new(content_length);                          \
            }                                                                                      \
            break;                                                                                 \
        case SmolRTSP_DeserializeResultErr:                                                        \
            self->state = SmolRTSP_RequestDeserializerStateErr;                                    \
            return res;                                                                            \
        case SmolRTSP_DeserializeResultNeedMore:                                                   \
            return res;                                                                            \
        }                                                                                          \
    } break

    SmolRTSP_DeserializeResult res;
    while (true) {
        switch (self->state) {
            ASSOC(Start, RequestLine, start_line);
            ASSOC(RequestLineParsed, HeaderMap, header_map);
            ASSOC(HeaderMapParsed, MessageBody, body);
        case SmolRTSP_RequestDeserializerStateMessageBodyParsed:
            return SmolRTSP_DeserializeResultOk;
        }
    }

#undef ASSOC
}
