#include <smolrtsp/writer.h>

#include <assert.h>
#include <string.h>

typedef char StringWriter;

static ssize_t StringWriter_write(VSelf, CharSlice99 data) {
    VSELF(StringWriter);
    assert(self);

    strncat(self, data.ptr, data.len);
    return data.len;
}

static int StringWriter_writef(VSelf, const char fmt[restrict], ...) {
    VSELF(StringWriter);
    assert(self);

    va_list ap;
    va_start(ap, fmt);

    const int ret = vsprintf(self + strlen(self), fmt, ap);
    va_end(ap);
    return ret;
}

static int StringWriter_vwritef(VSelf, const char fmt[restrict], va_list ap) {
    VSELF(StringWriter);
    assert(self);

    return vsprintf(self + strlen(self), fmt, ap);
}

impl(SmolRTSP_Writer, StringWriter);

SmolRTSP_Writer smolrtsp_string_writer(char *buffer) {
    assert(buffer);
    return DYN(StringWriter, SmolRTSP_Writer, buffer);
}
