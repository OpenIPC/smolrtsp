#ifndef SMOLRTSP_RESPONSE_H
#define SMOLRTSP_RESPONSE_H

#include <smolrtsp/header.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/reason_phrase.h>
#include <smolrtsp/rtsp_version.h>
#include <smolrtsp/status_code.h>

#include <stddef.h>

typedef struct {
    SmolRTSP_RTSPVersion version;
    SmolRTSP_StatusCode code;
    SmolRTSP_ReasonPhrase reason;
} SmolRTSP_ResponseLine;

typedef struct {
    SmolRTSP_ResponseLine start_line;
    size_t headers_count;
    SmolRTSP_Header *headers;
    SmolRTSP_MessageBody body;
} SmolRTSP_Response;

#endif // SMOLRTSP_RESPONSE_H
