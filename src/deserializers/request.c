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
    SmolRTSP_Cursor cursor;
    InnerDeserializers inner_deserializers;
};

SmolRTSP_RequestDeserializer *SmolRTSP_RequestDeserializer_new(void) {
    SmolRTSP_RequestDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->state = SmolRTSP_RequestDeserializerStateStart;
    self->cursor = NULL;
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

typedef SmolRTSP_DeserializeResult (*Deserializer)(
    void *restrict self, void *restrict entity, size_t size, const void *restrict data,
    size_t *restrict bytes_read);

SmolRTSP_DeserializeResult SmolRTSP_RequestDeserializer_deserialize(
    SmolRTSP_RequestDeserializer *restrict self, SmolRTSP_Request *restrict request, size_t size,
    const void *restrict data, size_t *restrict bytes_read) {
    if (self->state == SmolRTSP_RequestDeserializerStateMessageBodyParsed ||
        self->state == SmolRTSP_RequestDeserializerStateErr) {
        return self->state;
    }

    self->cursor = data;

#define ASSOC(current_state, next_entity_to_deserialize, var)                                      \
    case SmolRTSP_RequestDeserializerState##current_state: {                                       \
        size_t bytes_read_temp;                                                                    \
        res = SmolRTSP_##next_entity_to_deserialize##Deserializer_deserialize(                     \
            self->inner_deserializers.var, &request->var, size, data, &bytes_read_temp);           \
        *bytes_read += bytes_read_temp;                                                            \
        self->cursor += bytes_read_temp;                                                           \
        size -= bytes_read_temp;                                                                   \
                                                                                                   \
        switch (res) {                                                                             \
        case SmolRTSP_DeserializeResultOk:                                                         \
            self->state = SmolRTSP_RequestDeserializerState##next_entity_to_deserialize##Parsed;   \
            if (self->state == SmolRTSP_RequestDeserializerStateHeaderMapParsed) {                 \
                const char *content_length_value;                                                  \
                if ((content_length_value = SmolRTSP_HeaderMap_find(                               \
                         &request->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH)) == NULL) {    \
                    request->body.data = NULL;                                                     \
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
            break;                                                                                 \
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
