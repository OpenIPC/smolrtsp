#include "../../src/aux.h"
#include <smolrtsp/crlf.h>
#include <smolrtsp/deserializers/header_map.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header_map, SmolRTSP_HeaderMap expected) {
    SmolRTSP_Header headers[3];
    SmolRTSP_HeaderMapDeserializer *deser = SmolRTSP_HeaderMapDeserializer_new(3, headers);
    ASSERT_NE(deser, NULL);

    SmolRTSP_Slice data = SmolRTSP_Slice_from_str(header_map);
    const SmolRTSP_DeserializeResult res = SmolRTSP_HeaderMapDeserializer_deserialize(deser, &data);
    const SmolRTSP_HeaderMap inner = SmolRTSP_HeaderMapDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_HeaderMapDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(header_map));
    ASSERT(SmolRTSP_HeaderMap_eq(&inner, &expected));

    SmolRTSP_HeaderMapDeserializer_free(deser);
}

TEST(test_deserializers_header_map) {
    SmolRTSP_Header headers[] = {
        {
            SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
            SmolRTSP_Slice_from_str("10"),
        },
        {
            SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
            SmolRTSP_Slice_from_str("English"),
        },
        {
            SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
            SmolRTSP_Slice_from_str("application/octet-stream"),
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
