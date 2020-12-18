#include <smolrtsp/crlf.h>
#include <smolrtsp/deserializers/reason_phrase.h>

#include <string.h>

#include "../nala.h"

static void check(const char *phrase) {
    SmolRTSP_ReasonPhrase expected = SmolRTSP_Slice_from_str(phrase);
    expected.size -= strlen(SMOLRTSP_CRLF);

    SmolRTSP_ReasonPhraseDeserializer *deser = SmolRTSP_ReasonPhraseDeserializer_new();
    ASSERT_NE(deser, NULL);

    SmolRTSP_Slice data = SmolRTSP_Slice_from_str(phrase);
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_ReasonPhraseDeserializer_deserialize(deser, &data);
    const SmolRTSP_ReasonPhrase inner = SmolRTSP_ReasonPhraseDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_ReasonPhraseDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(phrase));
    ASSERT(SmolRTSP_Slice_eq(&inner, &expected));

    SmolRTSP_ReasonPhraseDeserializer_free(deser);
}

TEST(test_deserializers_reason_phrase) {
    check("Moved Temporarily" SMOLRTSP_CRLF);
    check("Forbidden" SMOLRTSP_CRLF);
    check("Header Field Not Valid for Resource" SMOLRTSP_CRLF);
}
