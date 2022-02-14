#include <smolrtsp/writer.h>

#include <assert.h>

#include <unistd.h>

void smolrtsp_write_slices(
    SmolRTSP_Writer w, size_t len, const CharSlice99 data[restrict static len]) {
    assert(w.self && w.vptr);

    for (size_t i = 0; i < len; i++) {
        VCALL(w, write, data[i]);
    }
}
