#include <smolrtsp/header_map.h>

#include "correctness.h"
#include "parsing.h"

#include <string.h>

SmolRTSP_HeaderMap SmolRTSP_HeaderMap_empty(void) {
    return (SmolRTSP_HeaderMap){.headers = NULL, .len = 0, .capacity = 0};
}

bool SmolRTSP_HeaderMap_find(
    SmolRTSP_HeaderMap self, CharSlice99 key, CharSlice99 *restrict value) {
    precondition(value);

    for (size_t i = 0; i < self.len; i++) {
        if (CharSlice99_primitive_eq(self.headers[i].key, key)) {
            *value = self.headers[i].value;
            return true;
        }
    }

    return false;
}

bool SmolRTSP_HeaderMap_key_is_present(SmolRTSP_HeaderMap self, CharSlice99 key) {
    CharSlice99 value;
    return SmolRTSP_HeaderMap_find(self, key, &value);
}

void SmolRTSP_HeaderMap_serialize(
    SmolRTSP_HeaderMap self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    for (size_t i = 0; i < self.len; i++) {
        SmolRTSP_Header_serialize(self.headers[i], user_writer, user_cx);
    }

    user_writer(SMOLRTSP_CRLF, user_cx);
}

SmolRTSP_DeserializeResult
SmolRTSP_HeaderMap_deserialize(SmolRTSP_HeaderMap *restrict self, CharSlice99 *restrict data) {
    precondition(self);
    precondition(data);

    while (true) {
        if (data->len < SMOLRTSP_CRLF.len) {
            return SmolRTSP_DeserializeResultPending;
        }

        if (CharSlice99_primitive_starts_with(*data, SMOLRTSP_CRLF)) {
            *data = CharSlice99_advance(*data, SMOLRTSP_CRLF.len);
            return SmolRTSP_DeserializeResultOk;
        }

        if (SmolRTSP_HeaderMap_is_full(*self)) {
            return SmolRTSP_DeserializeResultErr;
        }

        SmolRTSP_Header header;
        MATCH(SmolRTSP_Header_deserialize(&header, data));
        self->headers[self->len] = header;
        self->len++;
    }
}

bool SmolRTSP_HeaderMap_eq(SmolRTSP_HeaderMap lhs, SmolRTSP_HeaderMap rhs) {
    if (lhs.len != rhs.len) {
        return false;
    }

    const size_t len = lhs.len;

    for (size_t i = 0; i < len; i++) {
        if (!SmolRTSP_Header_eq(lhs.headers[i], rhs.headers[i])) {
            return false;
        }
    }

    return true;
}

bool SmolRTSP_HeaderMap_is_full(SmolRTSP_HeaderMap self) {
    return self.len == self.capacity;
}

void SmolRTSP_HeaderMap_dbg_to_file(SmolRTSP_HeaderMap self, FILE *stream) {
    precondition(stream);

    for (size_t i = 0; i < self.len; i++) {
        SmolRTSP_Header_dbg_to_file(self.headers[i], stream);
    }
}

void SmolRTSP_HeaderMap_dbg(SmolRTSP_HeaderMap self) {
    SmolRTSP_HeaderMap_dbg_to_file(self, stderr);
}
