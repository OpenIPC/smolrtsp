#include <smolrtsp/rtsp_version.h>

#include "correctness.h"
#include "parsing.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SmolRTSP_RTSPVersion SmolRTSP_RTSPVersion_new(uint_least8_t major, uint_least8_t minor) {
    return (SmolRTSP_RTSPVersion){.major = major, .minor = minor};
}

void SmolRTSP_RTSPVersion_serialize(
    SmolRTSP_RTSPVersion self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    const char *fmt = "RTSP/%" PRIuLEAST8 ".%" PRIuLEAST8;

    const size_t buffer_size = snprintf(NULL, 0, fmt, self.major, self.minor) + 1;

    char *buffer = malloc(buffer_size);
    invariant(buffer);
    snprintf(buffer, buffer_size, fmt, self.major, self.minor);

    user_writer(CharSlice99_from_str(buffer), user_cx);

    free(buffer);
}

SmolRTSP_DeserializeResult
SmolRTSP_RTSPVersion_deserialize(SmolRTSP_RTSPVersion *restrict self, CharSlice99 *restrict data) {
    precondition(self);
    precondition(data);

    MATCH(SmolRTSP_match_whitespaces(data));
    MATCH(SmolRTSP_match_str(data, "RTSP/"));

    CharSlice99 major = *data;
    MATCH(SmolRTSP_match_numeric(data));
    major = CharSlice99_from_ptrdiff(major.ptr, data->ptr);
    MATCH(SmolRTSP_match_char(data, '.'));
    CharSlice99 minor = *data;
    MATCH(SmolRTSP_match_numeric(data));
    minor = CharSlice99_from_ptrdiff(minor.ptr, data->ptr);

    uint_least8_t major_int, minor_int;
    char fmt[64];

    snprintf(fmt, sizeof(fmt), "%%%zd" SCNuLEAST8, major.len);
    if (sscanf(major.ptr, fmt, &major_int) != 1) {
        return SmolRTSP_DeserializeResultErr;
    }

    snprintf(fmt, sizeof(fmt), "%%%zd" SCNuLEAST8, minor.len);
    if (sscanf(minor.ptr, fmt, &minor_int) != 1) {
        return SmolRTSP_DeserializeResultErr;
    }

    self->major = major_int;
    self->minor = minor_int;

    return SmolRTSP_DeserializeResultOk;
}

bool SmolRTSP_RTSPVersion_eq(SmolRTSP_RTSPVersion lhs, SmolRTSP_RTSPVersion rhs) {
    return lhs.major == rhs.major && lhs.minor == rhs.minor;
}
