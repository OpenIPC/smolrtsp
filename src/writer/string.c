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

static int StringWriter_vwritef(VSelf, const char *restrict fmt, va_list ap) {
    VSELF(StringWriter);

    assert(self);
    assert(fmt);

    return vsprintf(self + strlen(self), fmt, ap);
}

static int StringWriter_writef(VSelf, const char *restrict fmt, ...) {
    VSELF(StringWriter);

    assert(self);
    assert(fmt);

    va_list ap;
    va_start(ap, fmt);

    const int ret = StringWriter_vwritef(self, fmt, ap);
    va_end(ap);

    return ret;
}

impl(SmolRTSP_Writer, StringWriter);

SmolRTSP_Writer smolrtsp_string_writer(char *buffer) {
    assert(buffer);
    return DYN(StringWriter, SmolRTSP_Writer, buffer);
}
