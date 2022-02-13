#include <smolrtsp/writer.h>

#include <stdio.h>
#include <string.h>

#include <unistd.h>

void smolrtsp_write_slices(
    SmolRTSP_Writer w, void *ctx, size_t len, const CharSlice99 data[restrict static len]) {
    for (size_t i = 0; i < len; i++) {
        w(data[i], ctx);
    }
}

void smolrtsp_char_buffer_writer(CharSlice99 data, void *buffer) {
    assert(buffer);
    strncat((char *)buffer, data.ptr, data.len);
}

void smolrtsp_file_stream_writer(CharSlice99 data, void *stream) {
    assert(stream);
    fwrite(data.ptr, sizeof(char), data.len, (FILE *)stream);
}

void smolrtsp_fd_writer(CharSlice99 data, void *fd_ptr) {
    assert(fd_ptr);
    write(*(int *)fd_ptr, data.ptr, data.len);
}
