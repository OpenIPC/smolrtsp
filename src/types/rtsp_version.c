#include <smolrtsp/types/rtsp_version.h>

#include "parsing.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <alloca.h>

ssize_t SmolRTSP_RtspVersion_serialize(
    const SmolRTSP_RtspVersion *restrict self, SmolRTSP_Writer w) {
    assert(self);
    assert(w.self && w.vptr);

    return VCALL(
        w, writef, "RTSP/%" PRIu8 ".%" PRIu8, self->major, self->minor);
}

SmolRTSP_ParseResult SmolRTSP_RtspVersion_parse(
    SmolRTSP_RtspVersion *restrict self, CharSlice99 input) {
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

    if (sscanf(CharSlice99_alloca_c_str(major), "%" SCNu8, &major_int) != 1) {
        return SmolRTSP_ParseResult_Failure(
            SmolRTSP_ParseError_TypeMismatch(SmolRTSP_ParseType_Int, major));
    }

    if (sscanf(CharSlice99_alloca_c_str(minor), "%" SCNu8, &minor_int) != 1) {
        return SmolRTSP_ParseResult_Failure(
            SmolRTSP_ParseError_TypeMismatch(SmolRTSP_ParseType_Int, minor));
    }

    self->major = major_int;
    self->minor = minor_int;

    return SmolRTSP_ParseResult_complete(input.ptr - backup.ptr);
}

bool SmolRTSP_RtspVersion_eq(
    const SmolRTSP_RtspVersion *restrict lhs,
    const SmolRTSP_RtspVersion *restrict rhs) {
    assert(lhs);
    assert(rhs);

    return lhs->major == rhs->major && lhs->minor == rhs->minor;
}
