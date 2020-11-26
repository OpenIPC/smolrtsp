#include "aux.h"
#include <smolrtsp/deserializers/method.h>
#include <smolrtsp/deserializers/request_uri.h>
#include <smolrtsp/deserializers/rtsp_version.h>
#include <smolrtsp/request_line.h>

#include <string.h>

void SmolRTSP_RequestLine_serialize(
    const SmolRTSP_RequestLine *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    SmolRTSP_Method_serialize(&self->method, user_writer, user_cx);
    SmolRTSP_RequestUri_serialize(&self->uri, user_writer, user_cx);
    SmolRTSP_RTSPVersion_serialize(&self->version, user_writer, user_cx);
    user_writer(strlen(SMOLRTSP_CRLF), SMOLRTSP_CRLF, user_cx);
}
