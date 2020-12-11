#include <smolrtsp/deserializers/request_uri.h>

#include <string.h>

#include "../nala.h"

static void check(const char *uri, SmolRTSP_RequestURI expected) {
    SmolRTSP_RequestURIDeserializer *deser = SmolRTSP_RequestURIDeserializer_new();
    ASSERT_NE(deser, NULL);

    SmolRTSP_DeserializeResult res =
        SmolRTSP_RequestURIDeserializer_deserialize(deser, SmolRTSP_Slice_new(uri, strlen(uri)));
    SmolRTSP_RequestURI inner = SmolRTSP_RequestURIDeserializer_inner(deser);
    size_t bytes_read = SmolRTSP_RequestURIDeserializer_bytes_read(deser);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(bytes_read, strlen(uri));
    ASSERT(SmolRTSP_RequestURI_eq(&inner, &expected));

    SmolRTSP_RequestURIDeserializer_free(deser);
}

TEST(test_deserializers_request_uri) {
    check("blah-blah-blah", (SmolRTSP_RequestURI){"blah-blah-blah"});
    check("http://example.com", (SmolRTSP_RequestURI){"http://example.com"});
}
