#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <smolrtsp/user_writer.h>

typedef struct {
    char *key;
    char *value;
} SmolRTSP_Header;

#define SMOLRTSP_HEADER_NAME_ACCEPT_RANGES      "Accept-Ranges"
#define SMOLRTSP_HEADER_NAME_CACHE_CONTROL      "Cache-Control"
#define SMOLRTSP_HEADER_NAME_CONNECTION         "Connection"
#define SMOLRTSP_HEADER_NAME_C_SEQ              "CSeq"
#define SMOLRTSP_HEADER_NAME_DATE               "Date"
#define SMOLRTSP_HEADER_NAME_MEDIA_PROPERTIES   "Media-Properties"
#define SMOLRTSP_HEADER_NAME_MEDIA_RANGE        "Media-Range"
#define SMOLRTSP_HEADER_NAME_PIPELINED_REQUESTS "Pipelined-Requests"
#define SMOLRTSP_HEADER_NAME_PROXY_SUPPORTED    "Proxy-Supported"
#define SMOLRTSP_HEADER_NAME_RANGE              "Range"
#define SMOLRTSP_HEADER_NAME_RTP_INFO           "RTP-Info"
#define SMOLRTSP_HEADER_NAME_SCALE              "Scale"
#define SMOLRTSP_HEADER_NAME_SEEK_STYLE         "Seek-Style"
#define SMOLRTSP_HEADER_NAME_SERVER             "Server"
#define SMOLRTSP_HEADER_NAME_SESSION            "Session"
#define SMOLRTSP_HEADER_NAME_SPEED              "Speed"
#define SMOLRTSP_HEADER_NAME_SUPPORTED          "Supported"
#define SMOLRTSP_HEADER_NAME_TIMESTAMP          "Timestamp"
#define SMOLRTSP_HEADER_NAME_TRANSPORT          "Transport"
#define SMOLRTSP_HEADER_NAME_USER_AGENT         "User-Agent"
#define SMOLRTSP_HEADER_NAME_VIA                "Via"

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header header, SmolRTSP_UserWriter user_writer, void *user_cx);

#endif // SMOLRTSP_HEADER_H
