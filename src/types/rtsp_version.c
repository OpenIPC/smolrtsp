#include <smolrtsp/types/rtsp_version.h>

#include "parsing.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t SmolRTSP_RtspVersion_serialize(SmolRTSP_RtspVersion self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    const char *fmt = "RTSP/%" PRIu8 ".%" PRIu8;

    const size_t buffer_size = snprintf(NULL, 0, fmt, self.major, self.minor) + 1;

    char *buffer = malloc(buffer_size);
    assert(buffer);
    snprintf(buffer, buffer_size, fmt, self.major, self.minor);

    const ssize_t result = VCALL(w, write, CharSlice99_from_str(buffer));
    free(buffer);
    return result;
}

SmolRTSP_ParseResult
SmolRTSP_RtspVersion_parse(SmolRTSP_RtspVersion *restrict self, CharSlice99 input) {
    assert(self);

    const CharSlice99 backup = input;

    MATCH(smolrtsp_match_whitespaces(input));
    MATCH(smolrtsp_match_str(input, "RTSP/"));

    CharSlice99 major = input;
    MATCH(smolrtsp_match_numeric(input));
    major = CharSlice99_from_ptrdiff(major.ptr, input.ptr);
    MATCH(smolrtsp_match_char(input, '.'));

    CharSlice99 minor = input;
    MATCH(smolrtsp_match_numeric(input));
    minor = CharSlice99_from_ptrdiff(minor.ptr, input.ptr);

    uint8_t major_int, minor_int;
    char fmt[64];

    snprintf(fmt, sizeof(fmt), "%%%zd" SCNu8, major.len);
    if (sscanf(major.ptr, fmt, &major_int) != 1) {
        return SmolRTSP_ParseResult_Failure(
            SmolRTSP_ParseError_TypeMismatch(SmolRTSP_ParseType_Int, major));
    }

    snprintf(fmt, sizeof(fmt), "%%%zd" SCNu8, minor.len);
    if (sscanf(minor.ptr, fmt, &minor_int) != 1) {
        return SmolRTSP_ParseResult_Failure(
            SmolRTSP_ParseError_TypeMismatch(SmolRTSP_ParseType_Int, minor));
    }

    self->major = major_int;
    self->minor = minor_int;

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_RtspVersion_eq(SmolRTSP_RtspVersion lhs, SmolRTSP_RtspVersion rhs) {
    return lhs.major == rhs.major && lhs.minor == rhs.minor;
}
