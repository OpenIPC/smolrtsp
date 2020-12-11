#include "../../src/aux.h"
#include <smolrtsp/deserializers/header.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header, SmolRTSP_Header expected) {
    SmolRTSP_HeaderDeserializer *deser = SmolRTSP_HeaderDeserializer_new();
    ASSERT_NE(deser, NULL);

    SmolRTSP_DeserializeResult res =
        SmolRTSP_HeaderDeserializer_deserialize(deser, SmolRTSP_Slice_new(header, strlen(header)));
    SmolRTSP_Header inner = SmolRTSP_HeaderDeserializer_inner(deser);
    size_t bytes_read = SmolRTSP_HeaderDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(header));
    ASSERT(SmolRTSP_Header_eq(&inner, &expected));

    SmolRTSP_HeaderDeserializer_free(deser);
}

TEST(test_deserializers_header) {
    SmolRTSP_Header expected = {
        SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
        strlen(SMOLRTSP_HEADER_NAME_CONTENT_LENGTH),
        "1556",
        strlen("1556"),
    };

    const char *content_length = SMOLRTSP_HEADER_NAME_CONTENT_LENGTH ": 1556" CRLF;

    check(content_length, expected);
}
