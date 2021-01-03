#include <smolrtsp/deserializers/status_code.h>

#include <string.h>

#include "../nala.h"

static void check(const char *code, SmolRTSP_StatusCode expected) {
    SmolRTSP_StatusCodeDeserializer *deser = SmolRTSP_StatusCodeDeserializer_new();
    ASSERT_NE(deser, NULL);

    Slice99 data = Slice99_from_str((char *)code);
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_StatusCodeDeserializer_deserialize(deser, &data);
    const SmolRTSP_StatusCode inner = SmolRTSP_StatusCodeDeserializer_inner(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(inner, expected);

    SmolRTSP_StatusCodeDeserializer_free(deser);
}

TEST(test_deserializers_status_code) {
    check("404", SMOLRTSP_STATUS_CODE_NOT_FOUND);
    check("100", SMOLRTSP_STATUS_CODE_CONTINUE);
    check("452", SMOLRTSP_STATUS_CODE_RESERVED);
    check("466", SMOLRTSP_STATUS_CODE_KEY_MANAGEMENT_ERROR);
}
