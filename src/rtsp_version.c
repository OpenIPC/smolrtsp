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

bool SmolRTSP_RTSPVersion_eq(
    const SmolRTSP_RTSPVersion *restrict lhs, const SmolRTSP_RTSPVersion *restrict rhs) {
    precondition(lhs);
    precondition(rhs);

    return lhs->major == rhs->major && lhs->minor == rhs->minor;
}
