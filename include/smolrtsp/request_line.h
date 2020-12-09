/**
 * @file
 * @brief An RTSP request line.
 */

#ifndef SMOLRTSP_REQUEST_LINE_H
#define SMOLRTSP_REQUEST_LINE_H

#include <smolrtsp/crlf.h>
#include <smolrtsp/method.h>
#include <smolrtsp/request_uri.h>
#include <smolrtsp/rtsp_version.h>

#include <stdbool.h>

typedef struct {
    SmolRTSP_Method method;
    SmolRTSP_RequestURI uri;
    SmolRTSP_RTSPVersion version;
    SmolRTSP_CRLF crlf;
} SmolRTSP_RequestLine;

void SmolRTSP_RequestLine_serialize(
    const SmolRTSP_RequestLine *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

bool SmolRTSP_RequestLine_eq(const SmolRTSP_RequestLine *lhs, const SmolRTSP_RequestLine *rhs);

#endif // SMOLRTSP_REQUEST_LINE_H
