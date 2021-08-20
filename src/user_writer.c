#include <smolrtsp/user_writer.h>

#include <stdio.h>
#include <string.h>

void smolrtsp_char_buffer_writer(CharSlice99 data, void *buffer) {
    strncat((char *)buffer, data.ptr, CharSlice99_size(data));
}

void smolrtsp_file_stream_writer(CharSlice99 data, void *stream) {
    fwrite(data.ptr, 1, CharSlice99_size(data), stream);
}
