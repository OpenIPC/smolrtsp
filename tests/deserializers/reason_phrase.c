#include <smolrtsp/deserializers/reason_phrase.h>

#include <string.h>

#include "../nala.h"

static void check(const char *phrase, SmolRTSP_ReasonPhrase expected) {
    SmolRTSP_ReasonPhraseDeserializer *deser = SmolRTSP_ReasonPhraseDeserializer_new();
    ASSERT_NE(deser, NULL);

    const SmolRTSP_DeserializeResult res = SmolRTSP_ReasonPhraseDeserializer_deserialize(
        deser, SmolRTSP_Slice_new(phrase, strlen(phrase)));
    const SmolRTSP_ReasonPhrase inner = SmolRTSP_ReasonPhraseDeserializer_inner(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_Slice_eq(&inner, &expected));

    SmolRTSP_ReasonPhraseDeserializer_free(deser);
}

TEST(test_deserializers_reason_phrase) {
    check("Moved Temporarily", SmolRTSP_Slice_from_str("Moved Temporarily"));
    check("Forbidden", SmolRTSP_Slice_from_str("Forbidden"));
    check(
        "Header Field Not Valid for Resource",
        SmolRTSP_Slice_from_str("Header Field Not Valid for Resource"));
}
