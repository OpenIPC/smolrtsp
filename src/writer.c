#include <smolrtsp/writer.h>

#include "macros.h"

#include <assert.h>

ssize_t smolrtsp_write_slices(
    SmolRTSP_Writer w, size_t len,
    const CharSlice99 data[restrict static len]) {
    assert(w.self && w.vptr);

    ssize_t result = 0, ret = 0;

    for (size_t i = 0; i < len; i++) {
        ret = VCALL(w, write, data[i]);
        CHK_WRITE_ERR(result, ret);
    }

    return result;
}
