#ifndef SMOLRTSP_REQUEST_URI_H
#define SMOLRTSP_REQUEST_URI_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/limits.h>

typedef char SmolRTSP_RequestURI[SMOLRTSP_REQUEST_URI_SIZE];

SmolRTSP_DeserializeResult SmolRTSP_StatusCode_deserialize(
    SmolRTSP_RequestURI *restrict uri, size_t size, const void *restrict data);

#endif // SMOLRTSP_REQUEST_URI_H
