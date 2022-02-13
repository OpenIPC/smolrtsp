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
