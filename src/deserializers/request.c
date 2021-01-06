#include "../correctness.h"
#include "../match.h"
#include <smolrtsp/deserializers/header_map.h>
#include <smolrtsp/deserializers/message_body.h>
#include <smolrtsp/deserializers/request.h>
#include <smolrtsp/deserializers/request_line.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct SmolRTSP_RequestDeserializer {
    SmolRTSP_RequestDeserializerState state;
    size_t bytes_read;

    struct {
        SmolRTSP_RequestLineDeserializer *start_line;
        SmolRTSP_HeaderMapDeserializer *header_map;
        SmolRTSP_MessageBodyDeserializer *body;
    } deserializers;
};

SmolRTSP_RequestDeserializer *SmolRTSP_RequestDeserializer_new(void) {
    SmolRTSP_RequestDeserializer *self;
    if ((self = malloc(sizeof(*self))) == NULL) {
        return NULL;
    }

    self->state = (SmolRTSP_RequestDeserializerState){
        .in_progress = SmolRTSP_RequestDeserializerStateInProgressRequestLine,
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

    INIT(self->deserializers.start_line = SmolRTSP_RequestLineDeserializer_new());
    INIT(self->deserializers.header_map = SmolRTSP_HeaderMapDeserializer_new());
    self->deserializers.body = NULL;

#undef INIT

    return self;
}

void SmolRTSP_RequestDeserializer_free(SmolRTSP_RequestDeserializer *self) {
    SmolRTSP_RequestLineDeserializer_free(self->deserializers.start_line);
    SmolRTSP_HeaderMapDeserializer_free(self->deserializers.header_map);
    SmolRTSP_MessageBodyDeserializer_free(self->deserializers.body);
    free(self);
}

SmolRTSP_RequestDeserializerState
SmolRTSP_RequestDeserializer_state(const SmolRTSP_RequestDeserializer *self) {
    precondition(self);

    return self->state;
}

size_t SmolRTSP_RequestDeserializer_bytes_read(SmolRTSP_RequestDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RequestDeserializer_deserialize(
    SmolRTSP_RequestDeserializer *restrict self, SmolRTSP_Request *restrict result,
    Slice99 *restrict data) {
    precondition(self);
    precondition(result);
    precondition(data);

    // TODO: Make a eDSL for this shit.
    if (self->state.in_progress == SmolRTSP_RequestDeserializerStateInProgressRequestLine) {
        MATCH(SmolRTSP_RequestLineDeserializer_deserialize(
            self->deserializers.start_line, &result->start_line, data));
        self->bytes_read +=
            SmolRTSP_RequestLineDeserializer_bytes_read(self->deserializers.start_line);
        self->state.in_progress = SmolRTSP_RequestDeserializerStateInProgressHeaderMap;
    }

    if (self->state.in_progress == SmolRTSP_RequestDeserializerStateInProgressHeaderMap) {
        MATCH(SmolRTSP_HeaderMapDeserializer_deserialize(
            self->deserializers.header_map, &result->header_map, data));
        self->bytes_read +=
            SmolRTSP_HeaderMapDeserializer_bytes_read(self->deserializers.header_map);
        self->state.in_progress = SmolRTSP_RequestDeserializerStateInProgressMessageBody;
    }

    bool is_found;
    Slice99 content_length_slice =
        SmolRTSP_HeaderMap_find(result->header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH, &is_found);

    size_t content_length;
    if (!is_found) {
        content_length = 0;
    } else if (sscanf(content_length_slice.ptr, "%zd", &content_length) != 1) {
        // TODO: Handle this error in a proper way.
        abort();
    }

    self->deserializers.body = SmolRTSP_MessageBodyDeserializer_new(content_length);

    if (self->state.in_progress == SmolRTSP_RequestDeserializerStateInProgressMessageBody) {
        MATCH(SmolRTSP_MessageBodyDeserializer_deserialize(
            self->deserializers.body, &result->body, data));
        self->bytes_read += SmolRTSP_MessageBodyDeserializer_bytes_read(self->deserializers.body);
        self->state.in_progress = SmolRTSP_RequestDeserializerStateInProgressDone;
    }

    return SmolRTSP_DeserializeResultOk;
}
