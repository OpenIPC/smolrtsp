/**
 * @file
 * @brief An RTSP request line.
 */

#ifndef SMOLRTSP_REQUEST_LINE_H
#define SMOLRTSP_REQUEST_LINE_H

#include <smolrtsp/method.h>
#include <smolrtsp/request_uri.h>
#include <smolrtsp/rtsp_version.h>

typedef struct {
    SmolRTSP_Method method;
    SmolRTSP_RequestURI uri;
    SmolRTSP_RTSPVersion version;
} SmolRTSP_RequestLine;

void SmolRTSP_RequestLine_serialize(
    const SmolRTSP_RequestLine *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

#endif // SMOLRTSP_REQUEST_LINE_H
