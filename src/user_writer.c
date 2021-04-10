#include <smolrtsp/user_writer.h>

#include <stdio.h>
#include <string.h>

void smolrtsp_char_buffer_writer(Slice99 data, void *buffer) {
    strncat((char *)buffer, data.ptr, Slice99_size(data));
}

void smolrtsp_file_stream_writer(Slice99 data, void *stream) {
    fwrite(data.ptr, 1, Slice99_size(data), stream);
}
