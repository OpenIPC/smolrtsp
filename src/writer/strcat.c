#include <smolrtsp/writer.h>

#include <assert.h>
#include <string.h>

typedef char StrcatWriter;

static void StrcatWriter_write(VSelf, CharSlice99 data) {
    VSELF(StrcatWriter);
    assert(self);

    strncat(self, data.ptr, data.len);
}

static int StrcatWriter_writef(VSelf, const char fmt[restrict], ...) {
    VSELF(StrcatWriter);
    assert(self);

    va_list ap;
    va_start(ap, fmt);

    const int ret = vsprintf(self + strlen(self), fmt, ap);
    va_end(ap);
    return ret;
}

static int StrcatWriter_vwritef(VSelf, const char fmt[restrict], va_list ap) {
    VSELF(StrcatWriter);
    assert(self);

    return vsprintf(self + strlen(self), fmt, ap);
}

impl(SmolRTSP_Writer, StrcatWriter);

SmolRTSP_Writer smolrtsp_strcat_writer(char *buffer) {
    assert(buffer);
    return DYN(StrcatWriter, SmolRTSP_Writer, buffer);
}
