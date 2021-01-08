#include <smolrtsp/header.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header, SmolRTSP_Header expected) {
    size_t bytes_read = 0;

    Slice99 data = Slice99_from_str((char *)header);
    SmolRTSP_Header result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_Header_deserialize(&result, &data, &bytes_read);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(header));
    ASSERT(SmolRTSP_Header_eq(&result, &expected));
}

TEST(test_deserializers_header) {
    SmolRTSP_Header expected = {
        SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
        Slice99_from_str("1556"),
    };

    const char *content_length = "Content-Length: 1556\r\n";

    check(content_length, expected);
}
