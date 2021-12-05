#include <smolrtsp/writer.h>

#include <stdio.h>
#include <string.h>

void smolrtsp_write_slices(
    SmolRTSP_Writer w, void *ctx, size_t len, const CharSlice99 data[restrict static len]) {
    for (size_t i = 0; i < len; i++) {
        w(data[i], ctx);
    }
}

void smolrtsp_char_buffer_writer(CharSlice99 data, void *buffer) {
    strncat((char *)buffer, data.ptr, CharSlice99_size(data));
}

void smolrtsp_file_stream_writer(CharSlice99 data, void *stream) {
    fwrite(data.ptr, 1, CharSlice99_size(data), stream);
}
