#include <smolrtsp/writer.h>

#include <assert.h>
#include <string.h>

#include <unistd.h>

void smolrtsp_write_slices(
    SmolRTSP_Writer w, size_t len, const CharSlice99 data[restrict static len]) {
    for (size_t i = 0; i < len; i++) {
        VCALL(w, write, data[i]);
    }
}

typedef char StrcatWriter;

static void StrcatWriter_write(VSelf, CharSlice99 data) {
    VSELF(StrcatWriter);
    assert(self);

    strncat(self, data.ptr, data.len);
}

static int StrcatWriter_printf(VSelf, const char format[restrict], ...) {
    VSELF(StrcatWriter);
    assert(self);

    va_list ap;
    va_start(ap, format);

    const int ret = vsprintf(self, format, ap);
    va_end(ap);
    return ret;
}

static int StrcatWriter_vprintf(VSelf, const char format[restrict], va_list ap) {
    VSELF(StrcatWriter);
    assert(self);

    return vsprintf(self, format, ap);
}

impl(SmolRTSP_Writer, StrcatWriter);

SmolRTSP_Writer smolrtsp_strcat_writer(char *buffer) {
    assert(buffer);
    return DYN(StrcatWriter, SmolRTSP_Writer, buffer);
}

typedef FILE FileWriter;

static void FileWriter_write(VSelf, CharSlice99 data) {
    VSELF(FileWriter);
    assert(self);

    fwrite(data.ptr, sizeof(char), data.len, self);
}

static int FileWriter_printf(VSelf, const char format[restrict], ...) {
    VSELF(FileWriter);
    assert(self);

    va_list ap;
    va_start(ap, format);

    const int ret = vfprintf(self, format, ap);
    va_end(ap);
    return ret;
}

static int FileWriter_vprintf(VSelf, const char format[restrict], va_list ap) {
    VSELF(FileWriter);
    assert(self);

    return vfprintf(self, format, ap);
}

impl(SmolRTSP_Writer, FileWriter);

SmolRTSP_Writer smolrtsp_file_writer(FILE *stream) {
    assert(stream);
    return DYN(FileWriter, SmolRTSP_Writer, stream);
}

typedef int FdWriter;

static void FdWriter_write(VSelf, CharSlice99 data) {
    VSELF(FdWriter);
    assert(self);

    write(*self, data.ptr, data.len);
}

static int FdWriter_printf(VSelf, const char format[restrict], ...) {
    VSELF(FdWriter);
    assert(self);

    va_list ap;
    va_start(ap, format);

    const int ret = vdprintf(*self, format, ap);
    va_end(ap);
    return ret;
}

static int FdWriter_vprintf(VSelf, const char format[restrict], va_list ap) {
    VSELF(FdWriter);
    assert(self);

    return vdprintf(*self, format, ap);
}

impl(SmolRTSP_Writer, FdWriter);

SmolRTSP_Writer smolrtsp_fd_writer(int *fd) {
    assert(fd);
    return DYN(FdWriter, SmolRTSP_Writer, fd);
}
