#include "parsing_aux.h"
#include <smolrtsp/status_code.h>

#include <inttypes.h>

SmolRTSP_DeserializeResult SmolRTSP_StatusCode_deserialize(
    SmolRTSP_StatusCode *restrict code, size_t size, const void *restrict data) {
    SmolRTSP_StatusCode parsed_code;
    SmolRTSP_DeserializeResult res =
        SmolRTSP_parse(6, size, data, "%" SCNuLEAST16, 1, &parsed_code);

    if (res == SmolRTSP_DeserializeResultOk) {
        *code = parsed_code;
    }

    return res;
}
