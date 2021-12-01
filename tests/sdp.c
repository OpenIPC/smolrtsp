#include <smolrtsp/sdp.h>

#include "nala/nala.h"

TEST(serialize_sdp_line) {
    char buffer[20] = {0};

    SmolRTSP_SdpLine_serialize(
        (SmolRTSP_SdpLine){.ty = 'a', .value = CharSlice99_from_str("abc")},
        smolrtsp_char_buffer_writer, buffer);
    ASSERT_EQ(strcmp(buffer, "a=abc\r\n"), 0);
}
