#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <user_writer.h>

typedef struct {
    char *key;
    char *value;
} SmolRTSP_Header;

#define DEF_HEADERS()                                                                              \
    X(AcceptRanges, "Accept-Ranges")                                                               \
    X(CacheControl, "Cache-Control")                                                               \
    X(Connection, "Connection")                                                                    \
    X(CSeq, "CSeq")                                                                                \
    X(Date, "Date")                                                                                \
    X(MediaProperties, "Media-Properties")                                                         \
    X(MediaRange, "Media-Range")                                                                   \
    X(PipelinedRequests, "Pipelined-Requests")                                                     \
    X(ProxySupported, "Proxy-Supported")                                                           \
    X(Range, "Range")                                                                              \
    X(RTPInfo, "RTP-Info")                                                                         \
    X(Scale, "Scale")                                                                              \
    X(SeekStyle, "Seek-Style")                                                                     \
    X(Server, "Server")                                                                            \
    X(Session, "Session")                                                                          \
    X(Speed, "Speed")                                                                              \
    X(Supported, "Supported")                                                                      \
    X(Timestamp, "Timestamp")                                                                      \
    X(Transport, "Transport")                                                                      \
    X(UserAgent, "User-Agent")                                                                     \
    X(Via, "Via")

#define X(header, stringification) SmolRTSP_Header##header,
typedef enum { DEF_HEADERS() } SmolRTSP_KnownHeader;
#undef X

#define X(header, stringification) [SmolRTSP_Header##header] = stringification,
static const char smolrtsp_header_names[][21] = {DEF_HEADERS()};
#undef X

#undef DEF_HEADERS

void
SmolRTSP_KnownHeader_serialize(SmolRTSP_KnownHeader header, SmolRTSP_UserWriter writer, void *user_cx);

#endif // SMOLRTSP_HEADER_H
