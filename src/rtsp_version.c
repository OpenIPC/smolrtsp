#include "correctness.h"
#include "parsing.h"
#include <smolrtsp/rtsp_version.h>

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void SmolRTSP_RTSPVersion_serialize(
    SmolRTSP_RTSPVersion self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    const Slice99 rtsp_slash = Slice99_from_str("RTSP/"), dot = Slice99_from_str(".");
    char minor[3], major[3];
    snprintf(minor, sizeof(minor), "%" PRIuLEAST8, self.minor);
    snprintf(major, sizeof(major), "%" PRIuLEAST8, self.major);

    user_writer(rtsp_slash, user_cx);
    user_writer(Slice99_from_str(minor), user_cx);
    user_writer(dot, user_cx);
    user_writer(Slice99_from_str(major), user_cx);
}

SmolRTSP_DeserializeResult
SmolRTSP_RTSPVersion_deserialize(SmolRTSP_RTSPVersion *restrict self, Slice99 *restrict data) {
    precondition(self);
    precondition(data);

    MATCH(SmolRTSP_match_whitespaces(data));
    MATCH(SmolRTSP_match_str(data, "RTSP/"));

    Slice99 major = *data;
    MATCH(SmolRTSP_match_numeric(data));
    major = Slice99_from_ptrdiff(major.ptr, data->ptr, sizeof(char));
    MATCH(SmolRTSP_match_char(data, '.'));
    Slice99 minor = *data;
    MATCH(SmolRTSP_match_numeric(data));
    minor = Slice99_from_ptrdiff(minor.ptr, data->ptr, sizeof(char));

    uint_least8_t major_int;
    char format[50];
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, major.len);
    if (sscanf(major.ptr, format, &major_int) != 1) {
        return SmolRTSP_DeserializeResultErr;
    }

    uint_least8_t minor_int;
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, minor.len);
    if (sscanf(minor.ptr, format, &minor_int) != 1) {
        return SmolRTSP_DeserializeResultErr;
    }

    self->major = major_int;
    self->minor = minor_int;

    return SmolRTSP_DeserializeResultOk;
}

bool SmolRTSP_RTSPVersion_eq(SmolRTSP_RTSPVersion lhs, SmolRTSP_RTSPVersion rhs) {
    return lhs.major == rhs.major && lhs.minor == rhs.minor;
}
