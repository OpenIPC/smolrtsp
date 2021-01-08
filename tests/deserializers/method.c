#include <smolrtsp/method.h>

#include <string.h>

#include "../nala.h"

static void check(const char *method, SmolRTSP_Method expected) {
    Slice99 data = Slice99_from_str((char *)method);
    size_t bytes_read = 0;
    SmolRTSP_Method result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_Method_deserialize(&result, &data, &bytes_read);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(method));
    ASSERT(Slice99_primitive_eq(result, expected));
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
