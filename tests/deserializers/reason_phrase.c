#include <smolrtsp/deserializers/reason_phrase.h>

#include <string.h>

#include "../nala.h"

static void check(const char *phrase, SmolRTSP_ReasonPhrase expected) {
    SmolRTSP_ReasonPhraseDeserializer *deser = SmolRTSP_ReasonPhraseDeserializer_new();
    ASSERT_NE(deser, NULL);

    SmolRTSP_DeserializeResult res =
        SmolRTSP_ReasonPhraseDeserializer_deserialize(deser, strlen(phrase), phrase);
    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);

    SmolRTSP_ReasonPhrase inner = SmolRTSP_ReasonPhraseDeserializer_inner(deser);
    ASSERT(SmolRTSP_ReasonPhrase_eq(&inner, &expected));

    SmolRTSP_ReasonPhraseDeserializer_free(deser);
}

TEST(test_deserializers_reason_phrase) {
    check("Moved Temporarily", (SmolRTSP_ReasonPhrase){"Moved Temporarily"});
    check("Forbidden", (SmolRTSP_ReasonPhrase){"Forbidden"});
    check(
        "Header Field Not Valid for Resource",
        (SmolRTSP_ReasonPhrase){"Header Field Not Valid for Resource"});
}
