#include <smolrtsp/deserializers/request_uri.h>

#include <string.h>

#include "../nala.h"

static void check(const char *uri, SmolRTSP_RequestURI expected) {
    SmolRTSP_RequestURIDeserializer *deser = SmolRTSP_RequestURIDeserializer_new();
    ASSERT_NE(deser, NULL);

    const SmolRTSP_DeserializeResult res =
        SmolRTSP_RequestURIDeserializer_deserialize(deser, SmolRTSP_Slice_new(uri, strlen(uri)));
    const SmolRTSP_RequestURI inner = SmolRTSP_RequestURIDeserializer_inner(deser);
    const size_t bytes_read = SmolRTSP_RequestURIDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(uri));
    ASSERT(SmolRTSP_Slice_eq(&inner, &expected));

    SmolRTSP_RequestURIDeserializer_free(deser);
}

TEST(test_deserializers_request_uri) {
    check("blah-blah-blah", SmolRTSP_Slice_from_str("blah-blah-blah"));
    check("http://example.com", SmolRTSP_Slice_from_str("http://example.com"));
}
