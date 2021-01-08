#include <smolrtsp/reason_phrase.h>

#include <string.h>

#include "../nala.h"

static void check(const char *phrase) {
    SmolRTSP_ReasonPhrase expected = Slice99_from_str((char *)phrase);
    expected.len -= strlen("\r\n");

    Slice99 data = Slice99_from_str((char *)phrase);
    SmolRTSP_ReasonPhrase result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_ReasonPhrase_deserialize(&result, &data);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(test_deserializers_reason_phrase) {
    check(
        "Moved Temporarily"
        "\r\n");
    check(
        "Forbidden"
        "\r\n");
    check(
        "Header Field Not Valid for Resource"
        "\r\n");
}
