#include <smolrtsp/deserializers/request_uri.h>

#include <string.h>

#include "../nala.h"

static void check(const char *uri, SmolRTSP_RequestURI expected) {
    SmolRTSP_RequestURIDeserializer *deser = SmolRTSP_RequestURIDeserializer_new();
    ASSERT_NE(deser, NULL);

    Slice99 data = Slice99_from_str((char *)uri);
    SmolRTSP_RequestURI result;
    const SmolRTSP_DeserializeResult res =
        SmolRTSP_RequestURIDeserializer_deserialize(deser, &result, &data);
    const size_t bytes_read = SmolRTSP_RequestURIDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(uri));
    ASSERT(Slice99_primitive_eq(result, expected));

    SmolRTSP_RequestURIDeserializer_free(deser);
}

TEST(test_deserializers_request_uri) {
    check("blah-blah-blah", Slice99_from_str("blah-blah-blah"));
    check("http://example.com", Slice99_from_str("http://example.com"));
}
