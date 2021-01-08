#include <smolrtsp/header.h>

#include <string.h>

#include "nala.h"

static void check(Slice99 header, SmolRTSP_Header expected) {
    SmolRTSP_Header result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_Header_deserialize(&result, &header);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_Header_eq(result, expected));
}

TEST(deserialize_header) {
    const SmolRTSP_Header expected = {
        SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
        Slice99_from_str("1556"),
    };

    check(Slice99_from_str("Content-Length: 1556\r\n"), expected);
}
