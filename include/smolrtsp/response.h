/**
 * @file
 *  An RTSP response.
 */

#ifndef SMOLRTSP_RESPONSE_H
#define SMOLRTSP_RESPONSE_H

#include <smolrtsp/header_map.h>
#include <smolrtsp/limits.h>
#include <smolrtsp/message_body.h>
#include <smolrtsp/reason_phrase.h>
#include <smolrtsp/rtsp_version.h>
#include <smolrtsp/status_code.h>

#include <stddef.h>

/**
 * An RTSP response line.
 */
typedef struct {
    SmolRTSP_RTSPVersion version;
    SmolRTSP_StatusCode code;
    SmolRTSP_ReasonPhrase reason;
} SmolRTSP_ResponseLine;

typedef struct {
    SmolRTSP_ResponseLine start_line;
    SmolRTSP_HeaderMap header_map;
    SmolRTSP_MessageBody body;
} SmolRTSP_Response;

#endif // SMOLRTSP_RESPONSE_H
