#include <smolrtsp/writer.h>

#include <assert.h>
#include <stdio.h>

typedef FILE FileWriter;

static ssize_t FileWriter_write(VSelf, CharSlice99 data) {
    VSELF(FileWriter);
    assert(self);

    const size_t ret = fwrite(data.ptr, sizeof(char), data.len, self);
    if (ferror(self)) {
        return -1;
    }

    return ret;
}

static int FileWriter_writef(VSelf, const char fmt[restrict], ...) {
    VSELF(FileWriter);
    assert(self);

    va_list ap;
    va_start(ap, fmt);

    const int ret = vfprintf(self, fmt, ap);
    va_end(ap);
    return ret;
}

static int FileWriter_vwritef(VSelf, const char fmt[restrict], va_list ap) {
    VSELF(FileWriter);
    assert(self);

    return vfprintf(self, fmt, ap);
}

impl(SmolRTSP_Writer, FileWriter);

SmolRTSP_Writer smolrtsp_file_writer(FILE *stream) {
    assert(stream);
    return DYN(FileWriter, SmolRTSP_Writer, stream);
}
