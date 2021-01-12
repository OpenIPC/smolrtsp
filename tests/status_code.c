#include <smolrtsp/status_code.h>

#include <string.h>

#include "nala.h"

static void check(Slice99 code, SmolRTSP_StatusCode expected) {
    SmolRTSP_StatusCode result;
    const SmolRTSP_DeserializeResult res = SmolRTSP_StatusCode_deserialize(&result, &code);

    ASSERT_EQ(res, SmolRTSP_DeserializeResultOk);
    ASSERT_EQ(result, expected);
}

TEST(deserialize_status_code) {
    check(Slice99_from_str("404 "), SMOLRTSP_STATUS_CODE_NOT_FOUND);
    check(Slice99_from_str("100 "), SMOLRTSP_STATUS_CODE_CONTINUE);
}
