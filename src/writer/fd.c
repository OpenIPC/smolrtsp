#include <smolrtsp/writer.h>

#include <assert.h>

typedef int FdWriter;

static ssize_t FdWriter_write(VSelf, CharSlice99 data) {
    VSELF(FdWriter);
    assert(self);

    return write(*self, data.ptr, data.len);
}

static int FdWriter_vwritef(VSelf, const char *restrict fmt, va_list ap) {
    VSELF(FdWriter);

    assert(self);
    assert(fmt);

    return vdprintf(*self, fmt, ap);
}

static int FdWriter_writef(VSelf, const char *restrict fmt, ...) {
    VSELF(FdWriter);

    assert(self);
    assert(fmt);

    va_list ap;
    va_start(ap, fmt);

    const int ret = FdWriter_vwritef(self, fmt, ap);
    va_end(ap);

    return ret;
}

impl(SmolRTSP_Writer, FdWriter);

SmolRTSP_Writer smolrtsp_fd_writer(int *fd) {
    assert(fd);
    return DYN(FdWriter, SmolRTSP_Writer, fd);
}
