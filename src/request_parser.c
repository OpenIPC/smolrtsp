#include "parsing_aux.h"
#include <smolrtsp/deserializers.h>
#include <smolrtsp/request_parser.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct SmolRTSP_RequestParser {
    SmolRTSP_ParseRequestResult state;
    SmolRTSP_Cursor cursor;

    struct {
        SmolRTSP_MethodDeserializer *method;
        SmolRTSP_RequestURIDeserializer *uri;
        SmolRTSP_RTSPVersionDeserializer *version;
        SmolRTSP_HeaderMapDeserializer *header_map;
        SmolRTSP_MessageBodyDeserializer *body;
    } deserializers;
};

SmolRTSP_RequestParser *SmolRTSP_RequestParser_new(void) {
    SmolRTSP_RequestParser *parser;
    if ((parser = malloc(sizeof(*parser))) == NULL) {
        return NULL;
    }

    parser->state = SmolRTSP_ParseRequestResultNothingParsed;
    parser->cursor = NULL;

    parser->deserializers.method = SmolRTSP_MethodDeserializer_new();
    parser->deserializers.uri = SmolRTSP_RequestURIDeserializer_new();
    parser->deserializers.version = SmolRTSP_RTSPVersionDeserializer_new();
    parser->deserializers.header_map = SmolRTSP_HeaderMapDeserializer_new();
    parser->deserializers.body = NULL;

    return parser;
}

void SmolRTSP_RequestParser_free(SmolRTSP_RequestParser *parser) {
    free(parser);
}

typedef SmolRTSP_DeserializeResult (*Deserializer)(
    void *restrict self, void *restrict entity, size_t size, const void *restrict data,
    size_t *restrict bytes_read);

SmolRTSP_ParseRequestResult SmolRTSP_RequestParser_parse(
    SmolRTSP_RequestParser *restrict parser, SmolRTSP_Request *restrict request, size_t size,
    const void *restrict data) {
    if (parser->state == SmolRTSP_ParseRequestResultMessageBodyParsed ||
        parser->state == SmolRTSP_ParseRequestResultErr) {
        return parser->state;
    }

    parser->cursor = data;

    typedef struct {
        Deserializer deserializer;
        void *self;
        void *entity;
        SmolRTSP_ParseRequestResult next_state_if_ok;
    } FSMTransition;

#define ASSOC(state, type, self, entity)                                                           \
    [SmolRTSP_ParseRequestResult##state] = {                                                       \
        (Deserializer)SmolRTSP_##type##Deserializer_deserialize,                                   \
        self,                                                                                      \
        entity,                                                                                    \
        .next_state_if_ok = SmolRTSP_ParseRequestResult##type##Parsed,                             \
    }

    const FSMTransition transition_table[] = {
        ASSOC(NothingParsed, Method, parser->deserializers.method, &request->start_line.method),
        ASSOC(MethodParsed, RequestURI, parser->deserializers.uri, &request->start_line.uri),
        ASSOC(
            RequestURIParsed, RTSPVersion, parser->deserializers.version,
            &request->start_line.version),
        ASSOC(HeaderParsed, HeaderMap, parser->deserializers.header_map, &request->header_map),
        ASSOC(HeaderMapParsed, MessageBody, parser->deserializers.body, &request->body),
    };

#undef ASSOC

    FSMTransition transition = transition_table[parser->state];

    while (true) {
        size_t bytes_read;
        SmolRTSP_DeserializeResult res = transition.deserializer(
            transition.self, transition.entity, size, parser->cursor, &bytes_read);
        parser->cursor += bytes_read;
        size -= bytes_read;

        switch (res) {
        case SmolRTSP_DeserializeResultOk:
            if (parser->state == SmolRTSP_ParseRequestResultHeaderMapParsed) {
                const char *content_length_value;
                if ((content_length_value = SmolRTSP_HeaderMap_find(
                         &request->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH)) == NULL) {
                    request->body.data = NULL;
                    parser->state = SmolRTSP_ParseRequestResultMessageBodyParsed;
                    return parser->state;
                }

                size_t content_length;
                if (sscanf(content_length_value, "%zd", &content_length) != 1) {
                    parser->state = SmolRTSP_ParseRequestResultErr;
                    return parser->state;
                }

                parser->deserializers.body = SmolRTSP_MessageBodyDeserializer_new(content_length);
            }

            parser->state = transition.next_state_if_ok;
            break;
        case SmolRTSP_DeserializeResultErr:
            parser->state = SmolRTSP_ParseRequestResultErr;
            return parser->state;
        case SmolRTSP_DeserializeResultNeedMore:
            return parser->state;
        }
    }
}
