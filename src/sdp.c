#include <smolrtsp/sdp.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void SmolRTSP_SdpLine_serialize(SmolRTSP_SdpLine self, SmolRTSP_Writer w, void *w_ctx) {
    assert(w);

    w(CharSlice99_new(&self.ty, 1), w_ctx);
    w(CharSlice99_from_str("="), w_ctx);
    w(self.value, w_ctx);
    w(SMOLRTSP_CRLF, w_ctx);
}
