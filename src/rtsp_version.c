#include "deser_aux.h"
#include <smolrtsp/limits.h>
#include <smolrtsp/rtsp_version.h>

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void SmolRTSP_RTSPVersion_serialize(
    const SmolRTSP_RTSPVersion *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    static const char rtsp_slash[] = "RTSP/", dot[] = ".";
    char minor[3], major[3];
    snprintf(minor, sizeof(minor), "%" PRIuLEAST8, self->minor);
    snprintf(major, sizeof(major), "%" PRIuLEAST8, self->major);

    user_writer(strlen(rtsp_slash), (const void *)rtsp_slash, user_cx);
    user_writer(strlen(minor), (const void *)minor, user_cx);
    user_writer(strlen(dot), (const void *)dot, user_cx);
    user_writer(strlen(major), (const void *)major, user_cx);
}

bool SmolRTSP_RTSPVersion_eq(const SmolRTSP_RTSPVersion *lhs, const SmolRTSP_RTSPVersion *rhs) {
    assert(lhs);
    assert(rhs);

    return lhs->major == rhs->major && lhs->minor == rhs->minor;
}
