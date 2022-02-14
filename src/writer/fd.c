#include <smolrtsp/writer.h>

#include <assert.h>

#include <unistd.h>

typedef int FdWriter;

static void FdWriter_write(VSelf, CharSlice99 data) {
    VSELF(FdWriter);
    assert(self);

    write(*self, data.ptr, data.len);
}

static int FdWriter_writef(VSelf, const char fmt[restrict], ...) {
    VSELF(FdWriter);
    assert(self);

    va_list ap;
    va_start(ap, fmt);

    const int ret = vdprintf(*self, fmt, ap);
    va_end(ap);
    return ret;
}

static int FdWriter_vwritef(VSelf, const char fmt[restrict], va_list ap) {
    VSELF(FdWriter);
    assert(self);

    return vdprintf(*self, fmt, ap);
}

impl(SmolRTSP_Writer, FdWriter);

SmolRTSP_Writer smolrtsp_fd_writer(int *fd) {
    assert(fd);
    return DYN(FdWriter, SmolRTSP_Writer, fd);
}
