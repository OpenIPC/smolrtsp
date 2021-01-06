#include <smolrtsp/deserializers/method.h>

#include <string.h>

#include "../nala.h"

static void check(const char *method, SmolRTSP_Method expected) {
    SmolRTSP_MethodDeserializer *deser = SmolRTSP_MethodDeserializer_new();
    ASSERT_NE(deser, NULL);

    Slice99 data = Slice99_from_str((char *)method);
    SmolRTSP_Method result;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_MethodDeserializer_deserialize(deser, &result, &data);
    const size_t bytes_read = SmolRTSP_MethodDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(method));
    ASSERT(Slice99_primitive_eq(result, expected));

    SmolRTSP_MethodDeserializer_free(deser);
}

TEST(test_deserializers_method) {
    check("OPTIONS", SMOLRTSP_METHOD_OPTIONS);
    check("DESCRIBE", SMOLRTSP_METHOD_DESCRIBE);
    check("ANNOUNCE", SMOLRTSP_METHOD_ANNOUNCE);
    check("SETUP", SMOLRTSP_METHOD_SETUP);
    check("PLAY", SMOLRTSP_METHOD_PLAY);
    check("PAUSE", SMOLRTSP_METHOD_PAUSE);
    check("TEARDOWN", SMOLRTSP_METHOD_TEARDOWN);
    check("GET_PARAMETER", SMOLRTSP_METHOD_GET_PARAMETER);
    check("SET_PARAMETER", SMOLRTSP_METHOD_SET_PARAMETER);
    check("REDIRECT", SMOLRTSP_METHOD_REDIRECT);
    check("RECORD", SMOLRTSP_METHOD_RECORD);
}
