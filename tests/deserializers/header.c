#include "../../src/aux.h"
#include <smolrtsp/crlf.h>
#include <smolrtsp/deserializers/header.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header, SmolRTSP_Header expected) {
    SmolRTSP_HeaderDeserializer *deser = SmolRTSP_HeaderDeserializer_new();
    ASSERT_NE(deser, NULL);

    const SmolRTSP_DeserializeResult res =
        SmolRTSP_HeaderDeserializer_deserialize(deser, SmolRTSP_Slice_new(header, strlen(header)));
    const SmolRTSP_Header inner = SmolRTSP_HeaderDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_HeaderDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(header));
    ASSERT(SmolRTSP_Header_eq(&inner, &expected));

    SmolRTSP_HeaderDeserializer_free(deser);
}

TEST(test_deserializers_header) {
    SmolRTSP_Header expected = {
        SmolRTSP_Slice_from_str(SMOLRTSP_HEADER_NAME_CONTENT_LENGTH),
        SmolRTSP_Slice_from_str("1556"),
    };

    const char *content_length = SMOLRTSP_HEADER_NAME_CONTENT_LENGTH ": 1556" SMOLRTSP_CRLF;

    check(content_length, expected);
}
