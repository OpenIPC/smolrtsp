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

static void FileWriter_lock(VSelf) {
    VSELF(FileWriter);
    (void)self;
}

static void FileWriter_unlock(VSelf) {
    VSELF(FileWriter);
    (void)self;
}

static size_t FileWriter_filled(VSelf) {
    VSELF(FileWriter);
    (void)self;
    return 0;
}

static int FileWriter_vwritef(VSelf, const char *restrict fmt, va_list ap) {
    VSELF(FileWriter);

    assert(self);
    assert(fmt);

    return vfprintf(self, fmt, ap);
}

static int FileWriter_writef(VSelf, const char *restrict fmt, ...) {
    VSELF(FileWriter);

    assert(self);
    assert(fmt);

    va_list ap;
    va_start(ap, fmt);

    const int ret = FileWriter_vwritef(self, fmt, ap);
    va_end(ap);

    return ret;
}

impl(SmolRTSP_Writer, FileWriter);

SmolRTSP_Writer smolrtsp_file_writer(FILE *stream) {
    assert(stream);
    return DYN(FileWriter, SmolRTSP_Writer, stream);
}
