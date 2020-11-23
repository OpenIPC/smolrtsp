#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

typedef struct {
    char key[SMOLRTSP_HEADER_KEY_SIZE];
    char value[SMOLRTSP_HEADER_VALUE_SIZE];
} SmolRTSP_Header;

// General-header fields:
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

// Request-header fields:
#define SMOLRTSP_HEADER_NAME_ACCEPT              "Accept"
#define SMOLRTSP_HEADER_NAME_ACCEPT_CREDENTIALS  "Accept-Credentials"
#define SMOLRTSP_HEADER_NAME_ACCEPT_ENCODING     "Accept-Encoding"
#define SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE     "Accept-Language"
#define SMOLRTSP_HEADER_NAME_AUTHORIZATION       "Authorization"
#define SMOLRTSP_HEADER_NAME_BANDWIDTH           "Bandwidth"
#define SMOLRTSP_HEADER_NAME_BLOCKSIZE           "Blocksize"
#define SMOLRTSP_HEADER_NAME_FROM                "From"
#define SMOLRTSP_HEADER_NAME_IF_MATCH            "If-Match"
#define SMOLRTSP_HEADER_NAME_IF_MODIFIED_SINCE   "If-Modified-Since"
#define SMOLRTSP_HEADER_NAME_IF_NONE_MATCH       "If-None-Match"
#define SMOLRTSP_HEADER_NAME_NOTIFY_REASON       "Notify-Reason"
#define SMOLRTSP_HEADER_NAME_PROXY_AUTHORIZATION "Proxy-Authorization"
#define SMOLRTSP_HEADER_NAME_PROXY_REQUIRE       "Proxy-Require"
#define SMOLRTSP_HEADER_NAME_REFERRER            "Referrer"
#define SMOLRTSP_HEADER_NAME_REQUEST_STATUS      "Request-Status"
#define SMOLRTSP_HEADER_NAME_REQUIRE             "Require"
#define SMOLRTSP_HEADER_NAME_TERMINATE_REASON    "Terminate-Reason"

// Response-header fields:
#define SMOLRTSP_HEADER_NAME_AUTHENTICATION_INFO    "Authorization-Info"
#define SMOLRTSP_HEADER_NAME_CONNECTION_CREDENTIALS "Connection-Credentials"
#define SMOLRTSP_HEADER_NAME_LOCATION               "Location"
#define SMOLRTSP_HEADER_NAME_M_TAG                  "M-Tag"
#define SMOLRTSP_HEADER_NAME_PROXY_AUTHENTICATE     "Proxy-Authenticate"
#define SMOLRTSP_HEADER_NAME_PUBLIC                 "Public"
#define SMOLRTSP_HEADER_NAME_RETRY_AFTER            "Retry-After"
#define SMOLRTSP_HEADER_NAME_UNSUPPORTED            "Unsupported"
#define SMOLRTSP_HEADER_NAME_WWW_AUTHENTICATE       "WWW-Authenticate"

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header header, SmolRTSP_UserWriter user_writer, void *user_cx);

SmolRTSP_DeserializeResult SmolRTSP_Header_deserialize(
    SmolRTSP_Header *restrict header, size_t size, const void *restrict data);

#endif // SMOLRTSP_HEADER_H
