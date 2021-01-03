#include <smolrtsp/deserializers/reason_phrase.h>

#include <string.h>

#include "../nala.h"

static void check(const char *phrase) {
    SmolRTSP_ReasonPhrase expected = Slice99_from_str((char *)phrase);
    expected.len -= strlen("\r\n");

    SmolRTSP_ReasonPhraseDeserializer *deser = SmolRTSP_ReasonPhraseDeserializer_new();
    ASSERT_NE(deser, NULL);

    Slice99 data = Slice99_from_str((char *)phrase);
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_ReasonPhraseDeserializer_deserialize(deser, &data);
    const SmolRTSP_ReasonPhrase inner = SmolRTSP_ReasonPhraseDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_ReasonPhraseDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(phrase));
    ASSERT(Slice99_primitive_eq(inner, expected));

    SmolRTSP_ReasonPhraseDeserializer_free(deser);
}

TEST(test_deserializers_reason_phrase) {
    check("Moved Temporarily" "\r\n");
    check("Forbidden" "\r\n");
    check("Header Field Not Valid for Resource" "\r\n");
}
