#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <user_writer.h>

typedef struct {
    char *key;
    char *value;
} SmolRTSP_Header;

#define SMOLRTSP_HEADER_ACCEPT_RANGES      "Accept-Ranges"
#define SMOLRTSP_HEADER_CACHE_CONTROL      "Cache-Control"
#define SMOLRTSP_HEADER_CONNECTION         "Connection"
#define SMOLRTSP_HEADER_C_SEQ              "CSeq"
#define SMOLRTSP_HEADER_DATE               "Date"
#define SMOLRTSP_HEADER_MEDIA_PROPERTIES   "Media-Properties"
#define SMOLRTSP_HEADER_MEDIA_RANGE        "Media-Range"
#define SMOLRTSP_HEADER_PIPELINED_REQUESTS "Pipelined-Requests"
#define SMOLRTSP_HEADER_PROXY_SUPPORTED    "Proxy-Supported"
#define SMOLRTSP_HEADER_RANGE              "Range"
#define SMOLRTSP_HEADER_RTP_INFO           "RTP-Info"
#define SMOLRTSP_HEADER_SCALE              "Scale"
#define SMOLRTSP_HEADER_SEEK_STYLE         "Seek-Style"
#define SMOLRTSP_HEADER_SERVER             "Server"
#define SMOLRTSP_HEADER_SESSION            "Session"
#define SMOLRTSP_HEADER_SPEED              "Speed"
#define SMOLRTSP_HEADER_SUPPORTED          "Supported"
#define SMOLRTSP_HEADER_TIMESTAMP          "Timestamp"
#define SMOLRTSP_HEADER_TRANSPORT          "Transport"
#define SMOLRTSP_HEADER_USER_AGENT         "User-Agent"
#define SMOLRTSP_HEADER_VIA                "Via"

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header header, SmolRTSP_UserWriter user_writer, void *user_cx);

#endif // SMOLRTSP_HEADER_H
