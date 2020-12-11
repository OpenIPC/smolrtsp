#include "../../src/aux.h"
#include <smolrtsp/deserializers/crlf.h>

#include <string.h>

#include "../nala.h"

TEST(test_crlf_ok) {
    SmolRTSP_CRLFDeserializer *deser = SmolRTSP_CRLFDeserializer_new();

    SmolRTSP_DeserializeResult res =
        SmolRTSP_CRLFDeserializer_deserialize(deser, SmolRTSP_Slice_from_str(CRLF));

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(SmolRTSP_CRLFDeserializer_bytes_read(deser), 2);

    SmolRTSP_CRLFDeserializer_free(deser);
}

TEST(test_crlf_err) {
    SmolRTSP_CRLFDeserializer *deser = SmolRTSP_CRLFDeserializer_new();

    SmolRTSP_DeserializeResult res =
        SmolRTSP_CRLFDeserializer_deserialize(deser, SmolRTSP_Slice_from_str("oa"));

    ASSERT_EQ(res, SmolRTSP_DeserializeResultErr);
    ASSERT_EQ(SmolRTSP_CRLFDeserializer_bytes_read(deser), 0);

    SmolRTSP_CRLFDeserializer_free(deser);
}

TEST(test_crlf_need_more) {
    SmolRTSP_CRLFDeserializer *deser = SmolRTSP_CRLFDeserializer_new();

    SmolRTSP_DeserializeResult res =
        SmolRTSP_CRLFDeserializer_deserialize(deser, SmolRTSP_Slice_from_str("\r"));

    ASSERT_EQ(res, SmolRTSP_DeserializeResultNeedMore);
    ASSERT_EQ(SmolRTSP_CRLFDeserializer_bytes_read(deser), 0);

    SmolRTSP_CRLFDeserializer_free(deser);
}
