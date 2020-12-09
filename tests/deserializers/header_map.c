#include "../../src/aux.h"
#include <smolrtsp/deserializers/header_map.h>

#include <string.h>

#include "../nala.h"

static void check(const char *header_map, SmolRTSP_HeaderMap expected) {
    SmolRTSP_HeaderMapDeserializer *deser = SmolRTSP_HeaderMapDeserializer_new();
    ASSERT_NE(deser, NULL);

    SmolRTSP_DeserializeResult res =
        SmolRTSP_HeaderMapDeserializer_deserialize(deser, strlen(header_map), header_map);
    SmolRTSP_HeaderMap inner = SmolRTSP_HeaderMapDeserializer_inner(deser);
    size_t bytes_read = SmolRTSP_HeaderMapDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(header_map));
    ASSERT(SmolRTSP_HeaderMap_eq(&inner, &expected));

    SmolRTSP_HeaderMapDeserializer_free(deser);
}

TEST(test_deserializers_header_map) {
    const SmolRTSP_HeaderMap expected = {
        .count = 3,
        .headers =
            {
                {
                    SMOLRTSP_HEADER_NAME_CONTENT_LENGTH,
                    strlen(SMOLRTSP_HEADER_NAME_CONTENT_LENGTH),
                    "50",
                    strlen("50"),
                },
                {
                    SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE,
                    strlen(SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE),
                    "English",
                    strlen("English"),
                },
                {
                    SMOLRTSP_HEADER_NAME_CONTENT_TYPE,
                    strlen(SMOLRTSP_HEADER_NAME_CONTENT_TYPE),
                    "application/octet-stream",
                    strlen("application/octet-stream"),
                },
            },
    };

    const char *header_map = SMOLRTSP_HEADER_NAME_CONTENT_LENGTH
        ": 50" CRLF SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE
        ": English" CRLF SMOLRTSP_HEADER_NAME_CONTENT_TYPE ": application/octet-stream" CRLF CRLF;

    check(header_map, expected);
}
