#ifndef SMOLRTSP_RESPONSE_H
#define SMOLRTSP_RESPONSE_H

#include <header.h>

#include <stddef.h>

typedef struct {

} SmolRTSP_ResponseLine;

typedef struct {
    SmolRTSP_ResponseLine start_line;
    size_t headers_count;
    SmolRTSP_Header *headers;
    const void *body;
} SmolRTSP_Response;

#endif // SMOLRTSP_RESPONSE_H
