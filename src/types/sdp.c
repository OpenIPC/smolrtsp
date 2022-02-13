#include <smolrtsp/types/sdp.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void SmolRTSP_SdpLine_serialize(SmolRTSP_SdpLine self, SmolRTSP_Writer w, void *w_ctx) {
    assert(w);

    SMOLRTSP_WRITE_SLICES(
        w, w_ctx,
        {
            CharSlice99_new(&self.ty, 1),
            CharSlice99_from_str("="),
            self.value,
            SMOLRTSP_CRLF,
        });
}

void smolrtsp_sdp_append(SmolRTSP_SdpType ty, const char *value, SmolRTSP_Writer w, void *w_ctx) {
    assert(w);

    SmolRTSP_SdpLine_serialize(
        (SmolRTSP_SdpLine){(ty), CharSlice99_from_str((char *)value)}, w, w_ctx);
}
