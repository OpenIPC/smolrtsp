/**
 * @file
 * @brief An RTSP request.
 */

#ifndef SMOLRTSP_REQUEST_H
#define SMOLRTSP_REQUEST_H

#include <smolrtsp/header_map.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/request_line.h>

typedef struct {
    SmolRTSP_RequestLine start_line;
    SmolRTSP_HeaderMap header_map;
    SmolRTSP_MessageBody body;
} SmolRTSP_Request;

#endif // SMOLRTSP_REQUEST_H
