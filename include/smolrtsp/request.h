#ifndef SMOLRTSP_REQUEST_H
#define SMOLRTSP_REQUEST_H

#include <smolrtsp/header.h>
#include <smolrtsp/message_body.h>

#include <stddef.h>

typedef struct {

} SmolRTSP_RequestLine;

typedef struct {
    SmolRTSP_RequestLine start_line;
    size_t headers_count;
    SmolRTSP_Header *headers;
    SmolRTSP_MessageBody body;
} SmolRTSP_Request;

#endif // SMOLRTSP_REQUEST_H
