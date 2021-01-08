#include <smolrtsp/reason_phrase.h>

#include <string.h>

#include "nala.h"

static void check(Slice99 phrase) {
    Slice99 expected = phrase;
    expected.len -= strlen("\r\n");

    SmolRTSP_ReasonPhrase result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_ReasonPhrase_deserialize(&result, &phrase);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(deserialize_reason_phrase) {
    check(Slice99_from_str("Moved Temporarily\r\n"));
    check(Slice99_from_str("Forbidden\r\n"));
    check(Slice99_from_str("Header Field Not Valid for Resource\r\n"));
}
