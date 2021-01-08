#include <smolrtsp/header_map.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header_map, SmolRTSP_HeaderMap expected) {
    SmolRTSP_HeaderMap result = {
        .headers = (SmolRTSP_Header[3]){0},
        .len = 0,
        .size = 3,
    };

    Slice99 data = Slice99_from_str((char *)header_map);
    const SmolRTSP_DeserializeResult res = SmolRTSP_HeaderMap_deserialize(&result, &data);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(SmolRTSP_HeaderMap_eq(result, expected));
}

TEST(test_deserializers_header_map) {
    SmolRTSP_Header headers[] = {
        {
            SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
            Slice99_from_str("10"),
        },
        {
            SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
            Slice99_from_str("English"),
        },
        {
            SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
            Slice99_from_str("application/octet-stream"),
        },
    };

    const SmolRTSP_HeaderMap expected = {
        .len = 3,
        .size = 3,
        .headers = headers,
    };

    const char *header_map =
        "Content-Length: 10\r\nAccept-Language: English\r\nContent-Type: "
        "application/octet-stream\r\n\r\n";

    check(header_map, expected);
}
