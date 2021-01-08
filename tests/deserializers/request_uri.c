#include <smolrtsp/request_uri.h>

#include <string.h>

#include "../nala.h"

static void check(const char *uri, SmolRTSP_RequestURI expected) {
    Slice99 data = Slice99_from_str((char *)uri);
    SmolRTSP_RequestURI result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_RequestURI_deserialize(&result, &data);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(test_deserializers_request_uri) {
    check("blah-blah-blah", Slice99_from_str("blah-blah-blah"));
    check("http://example.com", Slice99_from_str("http://example.com"));
}
