#include "../../src/aux.h"
#include <smolrtsp/deserializers/crlf.h>

#include <string.h>

#include "../nala.h"

TEST(test_crlf_ok) {
    SmolRTSP_CRLFDeserializer *deser = SmolRTSP_CRLFDeserializer_new();

    SmolRTSP_DeserializeResult res =
        SmolRTSP_CRLFDeserializer_deserialize(deser, strlen(CRLF), CRLF);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(SmolRTSP_CRLFDeserializer_bytes_read(deser), 2);

    SmolRTSP_CRLFDeserializer_free(deser);
}

TEST(test_crlf_err) {
    SmolRTSP_CRLFDeserializer *deser = SmolRTSP_CRLFDeserializer_new();

    SmolRTSP_DeserializeResult res = SmolRTSP_CRLFDeserializer_deserialize(deser, 2, "oa");

    ASSERT_EQ(res, SmolRTSP_DeserializeResultErr);
    ASSERT_EQ(SmolRTSP_CRLFDeserializer_bytes_read(deser), 0);

    SmolRTSP_CRLFDeserializer_free(deser);
}

TEST(test_crlf_need_more) {
    SmolRTSP_CRLFDeserializer *deser = SmolRTSP_CRLFDeserializer_new();

    SmolRTSP_DeserializeResult res = SmolRTSP_CRLFDeserializer_deserialize(deser, 1, "\r");

    ASSERT_EQ(res, SmolRTSP_DeserializeResultNeedMore);
    ASSERT_EQ(SmolRTSP_CRLFDeserializer_bytes_read(deser), 0);

    SmolRTSP_CRLFDeserializer_free(deser);
}
