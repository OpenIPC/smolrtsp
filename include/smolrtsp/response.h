#ifndef SMOLRTSP_RESPONSE_H
#define SMOLRTSP_RESPONSE_H

#include <smolrtsp/header.h>
#include <smolrtsp/message_body.h>

#include <stddef.h>

typedef struct {

} SmolRTSP_ResponseLine;

typedef struct {
    SmolRTSP_ResponseLine start_line;
    size_t headers_count;
    SmolRTSP_Header *headers;
    SmolRTSP_MessageBody body;
} SmolRTSP_Response;

#endif // SMOLRTSP_RESPONSE_H
