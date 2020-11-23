#include "parsing_aux.h"
#include <smolrtsp/method.h>

#include <string.h>

void SmolRTSP_Method_serialize(
    const SmolRTSP_Method method, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(strlen(method), (const void *)method, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_Method_deserialize(
    SmolRTSP_Method *restrict method, size_t size, const void *restrict data,
    size_t *restrict bytes_read) {
    SmolRTSP_Method parsed_method;

    SmolRTSP_DeserializeResult res =
        SmolRTSP_parse(SMOLRTSP_METHOD_SIZE, size, data, "%s%n", 2, parsed_method, bytes_read);

    if (res == SmolRTSP_DeserializeResultOk) {
        strncpy((char *)method, parsed_method, sizeof(parsed_method));
    }

    return res;
}
