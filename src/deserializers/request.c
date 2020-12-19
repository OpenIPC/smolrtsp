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
    SmolRTSP_Request inner;
    size_t bytes_read;

    struct {
        SmolRTSP_RequestLineDeserializer *start_line;
        SmolRTSP_HeaderMapDeserializer *header_map;
        SmolRTSP_MessageBodyDeserializer *body;
    } deserializers;
};

SmolRTSP_RequestDeserializer *
SmolRTSP_RequestDeserializer_new(size_t size, SmolRTSP_Header headers[static size]) {
    precondition(size >= 0);
    precondition(headers);

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
    INIT(self->deserializers.header_map = SmolRTSP_HeaderMapDeserializer_new(size, headers));
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

SmolRTSP_Request SmolRTSP_RequestDeserializer_inner(SmolRTSP_RequestDeserializer *self) {
    precondition(self);

    return self->inner;
}

size_t SmolRTSP_RequestDeserializer_bytes_read(SmolRTSP_RequestDeserializer *self) {
    precondition(self);

    return self->bytes_read;
}

SmolRTSP_DeserializeResult SmolRTSP_RequestDeserializer_deserialize(
    SmolRTSP_RequestDeserializer *restrict self, SmolRTSP_Slice *restrict data) {
    precondition(self);
    precondition(data);
    precondition(!SmolRTSP_Slice_is_null(*data));

    // TODO: Make an eDSL for this shit.
    if (self->state.in_progress == SmolRTSP_RequestDeserializerStateInProgressRequestLine) {
        MATCH(SmolRTSP_RequestLineDeserializer_deserialize(self->deserializers.start_line, data));
        self->inner.start_line =
            SmolRTSP_RequestLineDeserializer_inner(self->deserializers.start_line);
        self->bytes_read +=
            SmolRTSP_RequestLineDeserializer_bytes_read(self->deserializers.start_line);
        self->state.in_progress = SmolRTSP_RequestDeserializerStateInProgressHeaderMap;
    }

    if (self->state.in_progress == SmolRTSP_RequestDeserializerStateInProgressHeaderMap) {
        MATCH(SmolRTSP_HeaderMapDeserializer_deserialize(self->deserializers.header_map, data));
        self->inner.header_map =
            SmolRTSP_HeaderMapDeserializer_inner(self->deserializers.header_map);
        self->bytes_read +=
            SmolRTSP_HeaderMapDeserializer_bytes_read(self->deserializers.header_map);
        self->state.in_progress = SmolRTSP_RequestDeserializerStateInProgressMessageBody;
    }

    SmolRTSP_Slice content_length_slice =
        SmolRTSP_HeaderMap_find(&self->inner.header_map, SMOLRTSP_HEADER_NAME_CONTENT_LENGTH);

    size_t content_length;
    if (SmolRTSP_Slice_is_null(content_length_slice)) {
        content_length = 0;
    } else if (sscanf(content_length_slice.ptr, "%zd", &content_length) != 1) {
        // TODO: Handle this error in a proper way.
        abort();
    }

    self->deserializers.body = SmolRTSP_MessageBodyDeserializer_new(content_length);

    if (self->state.in_progress == SmolRTSP_RequestDeserializerStateInProgressMessageBody) {
        MATCH(SmolRTSP_MessageBodyDeserializer_deserialize(self->deserializers.body, data));
        self->inner.body = SmolRTSP_MessageBodyDeserializer_inner(self->deserializers.body);
        self->bytes_read += SmolRTSP_MessageBodyDeserializer_bytes_read(self->deserializers.body);
        self->state.in_progress = SmolRTSP_RequestDeserializerStateInProgressDone;
    }

    return SmolRTSP_DeserializeResultOk;
}
