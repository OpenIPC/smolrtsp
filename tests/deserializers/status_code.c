#include <smolrtsp/deserializers/status_code.h>

#include <string.h>

#include "../nala.h"

static void check_deserialization(const char *status_code, SmolRTSP_StatusCode expected) {
    SmolRTSP_StatusCodeDeserializer *deserializer = SmolRTSP_StatusCodeDeserializer_new();

    SmolRTSP_DeserializeResult res =
        SmolRTSP_StatusCodeDeserializer_deserialize(deserializer, strlen(status_code), status_code);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);

    SmolRTSP_StatusCode inner = SmolRTSP_StatusCodeDeserializer_inner(deserializer);
    ASSERT_EQ(inner, expected);

    SmolRTSP_StatusCodeDeserializer_free(deserializer);
}

void test_deserializers_status_code(void) {
    check_deserialization("404", 404);
}
