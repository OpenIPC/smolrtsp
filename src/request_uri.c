#include "parsing_aux.h"
#include <smolrtsp/request_uri.h>

#include <string.h>

SmolRTSP_DeserializeResult SmolRTSP_RequestURI_deserialize(
    SmolRTSP_RequestURI *restrict uri, size_t size, const void *restrict data,
    size_t *restrict bytes_read) {
    SmolRTSP_RequestURI parsed_uri;

    SmolRTSP_DeserializeResult res =
        SmolRTSP_parse(SMOLRTSP_REQUEST_URI_SIZE, size, data, "%s%n", 2, parsed_uri, bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        strncpy((char *)uri, parsed_uri, sizeof(parsed_uri));
    }

    return res;
}
