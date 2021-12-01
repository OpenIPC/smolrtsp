#include <smolrtsp/rtsp_version.h>

#include "parsing.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SmolRTSP_RtspVersion SmolRTSP_RtspVersion_new(uint_least8_t major, uint_least8_t minor) {
    return (SmolRTSP_RtspVersion){.major = major, .minor = minor};
}

void SmolRTSP_RtspVersion_serialize(
    SmolRTSP_RtspVersion self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(user_writer);

    const char *fmt = "RTSP/%" PRIuLEAST8 ".%" PRIuLEAST8;

    const size_t buffer_size = snprintf(NULL, 0, fmt, self.major, self.minor) + 1;

    char *buffer = malloc(buffer_size);
    assert(buffer);
    snprintf(buffer, buffer_size, fmt, self.major, self.minor);

    user_writer(CharSlice99_from_str(buffer), user_cx);

    free(buffer);
}

SmolRTSP_DeserializeResult
SmolRTSP_RtspVersion_deserialize(SmolRTSP_RtspVersion *restrict self, CharSlice99 *restrict data) {
    assert(self);
    assert(data);

    MATCH(smolrtsp_match_whitespaces(data));
    MATCH(smolrtsp_match_str(data, "RTSP/"));

    CharSlice99 major = *data;
    MATCH(smolrtsp_match_numeric(data));
    major = CharSlice99_from_ptrdiff(major.ptr, data->ptr);
    MATCH(smolrtsp_match_char(data, '.'));
    CharSlice99 minor = *data;
    MATCH(smolrtsp_match_numeric(data));
    minor = CharSlice99_from_ptrdiff(minor.ptr, data->ptr);

    uint_least8_t major_int, minor_int;
    char fmt[64];

    snprintf(fmt, sizeof(fmt), "%%%zd" SCNuLEAST8, major.len);
    if (sscanf(major.ptr, fmt, &major_int) != 1) {
        return SmolRTSP_DeserializeResult_Err;
    }

    snprintf(fmt, sizeof(fmt), "%%%zd" SCNuLEAST8, minor.len);
    if (sscanf(minor.ptr, fmt, &minor_int) != 1) {
        return SmolRTSP_DeserializeResult_Err;
    }

    self->major = major_int;
    self->minor = minor_int;

    return SmolRTSP_DeserializeResult_Ok;
}

bool SmolRTSP_RtspVersion_eq(SmolRTSP_RtspVersion lhs, SmolRTSP_RtspVersion rhs) {
    return lhs.major == rhs.major && lhs.minor == rhs.minor;
}
