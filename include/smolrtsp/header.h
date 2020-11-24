#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

typedef struct {
    char key[SMOLRTSP_HEADER_KEY_SIZE];
    char value[SMOLRTSP_HEADER_VALUE_SIZE];
} SmolRTSP_Header;

#define SMOLRTSP_HEADER_NAME_ACCEPT             "Accept"
#define SMOLRTSP_HEADER_NAME_ACCEPT_ENCODING    "Accept-Encoding"
#define SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE    "Accept-Language"
#define SMOLRTSP_HEADER_NAME_ALLOW              "Allow"
#define SMOLRTSP_HEADER_NAME_AUTHORIZATION      "Authorization"
#define SMOLRTSP_HEADER_NAME_BANDWIDTH          "Bandwidth"
#define SMOLRTSP_HEADER_NAME_BLOCKSIZE          "Blocksize"
#define SMOLRTSP_HEADER_NAME_CACHE_CONTROL      "Cache-Control"
#define SMOLRTSP_HEADER_NAME_CONFERENCE         "Conference"
#define SMOLRTSP_HEADER_NAME_CONNECTION         "Connection"
#define SMOLRTSP_HEADER_NAME_CONTENT_BASE       "Content-Base"
#define SMOLRTSP_HEADER_NAME_CONTENT_ENCODING   "Content-Encoding"
#define SMOLRTSP_HEADER_NAME_CONTENT_LANGUAGE   "Content-Language"
#define SMOLRTSP_HEADER_NAME_CONTENT_LENGTH     "Content-Length"
#define SMOLRTSP_HEADER_NAME_CONTENT_LOCATION   "Content-Location"
#define SMOLRTSP_HEADER_NAME_CONTENT_TYPE       "Content-Type"
#define SMOLRTSP_HEADER_NAME_C_SEQ              "CSeq"
#define SMOLRTSP_HEADER_NAME_DATE               "Date"
#define SMOLRTSP_HEADER_NAME_EXPIRES            "Expires"
#define SMOLRTSP_HEADER_NAME_FROM               "From"
#define SMOLRTSP_HEADER_NAME_IF_MODIFIED_SINCE  "If-Modified-Since"
#define SMOLRTSP_HEADER_NAME_LAST_MODIFIED      "Last-Modified"
#define SMOLRTSP_HEADER_NAME_PROXY_AUTHENTICATE "Proxy-Authenticate"
#define SMOLRTSP_HEADER_NAME_PROXY_REQUIRE      "Proxy-Require"
#define SMOLRTSP_HEADER_NAME_PUBLIC             "Public"
#define SMOLRTSP_HEADER_NAME_RANGE              "Range"
#define SMOLRTSP_HEADER_NAME_REFERER            "Referrer"
#define SMOLRTSP_HEADER_NAME_REQUIRE            "Require"
#define SMOLRTSP_HEADER_NAME_RETRY_AFTER        "Retry-After"
#define SMOLRTSP_HEADER_NAME_RTP_INFO           "RTP-Info"
#define SMOLRTSP_HEADER_NAME_SCALE              "Scale"
#define SMOLRTSP_HEADER_NAME_SESSION            "Session"
#define SMOLRTSP_HEADER_NAME_SERVER             "Server"
#define SMOLRTSP_HEADER_NAME_SPEED              "Speed"
#define SMOLRTSP_HEADER_NAME_TRANSPORT          "Transport"
#define SMOLRTSP_HEADER_NAME_UNSUPPORTED        "Unsupported"
#define SMOLRTSP_HEADER_NAME_USER_AGENT         "User-Agent"
#define SMOLRTSP_HEADER_NAME_VIA                "Via"
#define SMOLRTSP_HEADER_NAME_WWW_AUTHENTICATE   "WWW-Authenticate"

void SmolRTSP_Header_serialize(
    const SmolRTSP_Header header, SmolRTSP_UserWriter user_writer, void *user_cx);

#endif // SMOLRTSP_HEADER_H
