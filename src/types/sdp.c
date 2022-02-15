#include <smolrtsp/types/sdp.h>

#include "../macros.h"
#include "parsing.h"
#include <smolrtsp/util.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t SmolRTSP_SdpLine_serialize(SmolRTSP_SdpLine self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    return SMOLRTSP_WRITE_SLICES(
        w, {
               CharSlice99_new(&self.ty, 1),
               CharSlice99_from_str("="),
               self.value,
               SMOLRTSP_CRLF,
           });
}

ssize_t smolrtsp_sdp_printf(SmolRTSP_SdpType ty, SmolRTSP_Writer w, const char fmt[restrict], ...) {
    assert(w.self && w.vptr);
    assert(fmt);

    ssize_t result = 0;

    va_list ap;
    va_start(ap, fmt);

    CHK_WRITE_ERR(result, VCALL(w, writef, "%c=", ty));
    CHK_WRITE_ERR(result, VCALL(w, vwritef, fmt, ap));
    CHK_WRITE_ERR(result, VCALL(w, write, SMOLRTSP_CRLF));

    va_end(ap);

    return result;
}
