#include <smolrtsp/sdp.h>

#include "parsing.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void SmolRTSP_SdpLine_serialize(
    SmolRTSP_SdpLine self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(user_cx);

    user_writer(CharSlice99_new(&self.ty, 1), user_cx);
    user_writer(CharSlice99_from_str("="), user_cx);
    user_writer(self.value, user_cx);
    user_writer(SMOLRTSP_CRLF, user_cx);
}
