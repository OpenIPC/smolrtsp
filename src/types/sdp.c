#include <smolrtsp/types/sdp.h>

#include "crlf.h"
#include "parsing.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void SmolRTSP_SdpLine_serialize(SmolRTSP_SdpLine self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    SMOLRTSP_WRITE_SLICES(
        w, {
               CharSlice99_new(&self.ty, 1),
               CharSlice99_from_str("="),
               self.value,
               SMOLRTSP_CRLF,
           });
}

void smolrtsp_sdp_printf(SmolRTSP_SdpType ty, SmolRTSP_Writer w, const char fmt[restrict], ...) {
    assert(w.self && w.vptr);

    va_list ap;
    va_start(ap, fmt);

    VCALL(w, printf, "%c=", ty);
    VCALL(w, vprintf, fmt, ap);
    VCALL(w, write, SMOLRTSP_CRLF);

    va_end(ap);
}
