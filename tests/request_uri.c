#include <smolrtsp/request_uri.h>

#include <string.h>

#include "nala.h"

static void check(Slice99 uri) {
    SmolRTSP_RequestURI expected = uri;
    SmolRTSP_RequestURI result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_RequestURI_deserialize(&result, &uri);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT(Slice99_primitive_eq(result, expected));
}

TEST(deserialize_request_uri) {
    check(Slice99_from_str("blah-blah-blah"));
    check(Slice99_from_str("http://example.com"));
}
