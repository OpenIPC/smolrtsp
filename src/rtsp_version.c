#include "correctness.h"
#include "match.h"
#include <smolrtsp/rtsp_version.h>

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void SmolRTSP_RTSPVersion_serialize(
    const SmolRTSP_RTSPVersion *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(self);
    precondition(user_writer);

    static const char rtsp_slash[] = "RTSP/", dot[] = ".";
    char minor[3], major[3];
    snprintf(minor, sizeof(minor), "%" PRIuLEAST8, self->minor);
    snprintf(major, sizeof(major), "%" PRIuLEAST8, self->major);

    user_writer(strlen(rtsp_slash), rtsp_slash, user_cx);
    user_writer(strlen(minor), minor, user_cx);
    user_writer(strlen(dot), dot, user_cx);
    user_writer(strlen(major), major, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_RTSPVersion_deserialize(
    SmolRTSP_RTSPVersion *restrict self, Slice99 *restrict data, size_t *restrict bytes_read) {
    precondition(self);
    precondition(data);
    precondition(bytes_read);

    size_t bytes_read_temp = 0;

    MATCH(SmolRTSP_match_whitespaces(data, &bytes_read_temp));
    MATCH(SmolRTSP_match_str(data, &bytes_read_temp, "RTSP/"));

    const char *major = data->ptr;
    MATCH(SmolRTSP_match_numeric(data, &bytes_read_temp));
    const size_t major_size = (const char *)data->ptr - major;
    MATCH(SmolRTSP_match_char(data, &bytes_read_temp, '.'));
    const char *minor = data->ptr;
    MATCH(SmolRTSP_match_numeric(data, &bytes_read_temp));
    const size_t minor_size = (const char *)data->ptr - minor;

    uint_least8_t major_int;
    char format[50];
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, major_size);
    int rc = sscanf(major, format, &major_int);
    precondition(rc == 1);

    uint_least8_t minor_int;
    snprintf(format, sizeof(format), "%%%zd" SCNuLEAST16, minor_size);
    rc = sscanf(minor, format, &minor_int);
    precondition(rc == 1);

    *bytes_read += bytes_read_temp;
    self->major = major_int;
    self->minor = minor_int;

    return SmolRTSP_DeserializeResultOk;
}

bool SmolRTSP_RTSPVersion_eq(
    const SmolRTSP_RTSPVersion *restrict lhs, const SmolRTSP_RTSPVersion *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return lhs->major == rhs->major && lhs->minor == rhs->minor;
}
