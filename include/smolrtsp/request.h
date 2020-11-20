#ifndef SMOLRTSP_REQUEST_H
#define SMOLRTSP_REQUEST_H

#include <smolrtsp/header.h>
#include <smolrtsp/limits.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/method.h>
#include <smolrtsp/request_uri.h>
#include <smolrtsp/rtsp_version.h>

#include <stddef.h>

typedef struct {
    SmolRTSP_Method method;
    SmolRTSP_RequestURI uri;
    SmolRTSP_RTSPVersion version;
} SmolRTSP_RequestLine;

typedef struct {
    SmolRTSP_RequestLine start_line;
    size_t headers_count;
    SmolRTSP_Header headers[SMOLRTSP_MAX_REQUEST_HEADERS];
    SmolRTSP_MessageBody body;
} SmolRTSP_Request;

#endif // SMOLRTSP_REQUEST_H
