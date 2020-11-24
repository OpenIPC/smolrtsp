#include "../parsing_aux.h"
#include <smolrtsp/deserializers.h>
#include <smolrtsp/deserializers/request.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    SmolRTSP_MethodDeserializer *method;
    SmolRTSP_RequestURIDeserializer *uri;
    SmolRTSP_RTSPVersionDeserializer *version;
    SmolRTSP_HeaderMapDeserializer *header_map;
    SmolRTSP_MessageBodyDeserializer *body;
} InnerDeserializers;

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

    self->state = SmolRTSP_RequestDeserializerStateNothingParsed;
    self->cursor = NULL;

    self->inner_deserializers = (InnerDeserializers){
        .method = SmolRTSP_MethodDeserializer_new(),
        .uri = SmolRTSP_RequestURIDeserializer_new(),
        .version = SmolRTSP_RTSPVersionDeserializer_new(),
        .header_map = SmolRTSP_HeaderMapDeserializer_new(),
        .body = NULL,
    };

    return self;
}

void SmolRTSP_RequestDeserializer_free(SmolRTSP_RequestDeserializer *self) {
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

    typedef struct {
        Deserializer deserializer;
        void *self;
        void *entity;
        SmolRTSP_RequestDeserializerState next_state_if_ok;
    } FSMTransition;

#define ASSOC(state, type, self, entity)                                                           \
    [SmolRTSP_RequestDeserializerState##state] = {                                                 \
        (Deserializer)SmolRTSP_##type##Deserializer_deserialize,                                   \
        self,                                                                                      \
        entity,                                                                                    \
        .next_state_if_ok = SmolRTSP_RequestDeserializerState##type##Parsed,                       \
    }

    const FSMTransition transition_table[] = {
        ASSOC(NothingParsed, Method, self->inner_deserializers.method, &request->start_line.method),
        ASSOC(MethodParsed, RequestURI, self->inner_deserializers.uri, &request->start_line.uri),
        ASSOC(
            RequestURIParsed, RTSPVersion, self->inner_deserializers.version,
            &request->start_line.version),
        ASSOC(HeaderParsed, HeaderMap, self->inner_deserializers.header_map, &request->header_map),
        ASSOC(HeaderMapParsed, MessageBody, self->inner_deserializers.body, &request->body),
    };

#undef ASSOC

    FSMTransition transition = transition_table[self->state];

    while (true) {
        size_t bytes_read;
        SmolRTSP_DeserializeResult res = transition.deserializer(
            transition.self, transition.entity, size, self->cursor, &bytes_read);
        self->cursor += bytes_read;
        size -= bytes_read;

        switch (res) {
        case SmolRTSP_DeserializeResultOk:
            if (self->state == SmolRTSP_RequestDeserializerStateHeaderMapParsed) {
                const char *content_length_value;
                if ((content_length_value = SmolRTSP_HeaderMap_find(
                         &request->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH)) == NULL) {
                    request->body.data = NULL;
                    self->state = SmolRTSP_RequestDeserializerStateMessageBodyParsed;
                    return self->state;
                }

                size_t content_length;
                if (sscanf(content_length_value, "%zd", &content_length) != 1) {
                    self->state = SmolRTSP_RequestDeserializerStateErr;
                    return self->state;
                }

                self->inner_deserializers.body =
                    SmolRTSP_MessageBodyDeserializer_new(content_length);
            }

            self->state = transition.next_state_if_ok;
            break;
        case SmolRTSP_DeserializeResultErr:
            self->state = SmolRTSP_RequestDeserializerStateErr;
            return self->state;
        case SmolRTSP_DeserializeResultNeedMore:
            return self->state;
        }
    }
}
