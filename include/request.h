#ifndef SMOLRTSP_REQUEST_H
#define SMOLRTSP_REQUEST_H

#include <header.h>

#include <stddef.h>

typedef struct {

} SmolRTSP_RequestLine;

typedef struct {
    SmolRTSP_RequestLine start_line;
    size_t headers_count;
    SmolRTSP_Header *headers;
    const void *body;
} SmolRTSP_Request;

#endif // SMOLRTSP_REQUEST_H
