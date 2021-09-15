#include <smolrtsp/sdp.h>

#include "nala/nala.h"

#define TEST_VARIANTS                                                                              \
    do {                                                                                           \
        X(SmolRTSP_SdpLineType_Version, "v");                                                      \
        X(SmolRTSP_SdpLineType_Origin, "o");                                                       \
        X(SmolRTSP_SdpLineType_SessionName, "s");                                                  \
        X(SmolRTSP_SdpLineType_Connection, "c");                                                   \
        X(SmolRTSP_SdpLineType_Media, "m");                                                        \
        X(SmolRTSP_SdpLineType_Attr, "a");                                                         \
        X(SmolRTSP_SdpLineType_Time, "t");                                                         \
    } while (0)

TEST(sdp_line_type_str) {
#define X(variant, expected) ASSERT_EQ(strcmp(SmolRTSP_SdpLineType_str(variant), expected), 0)

    TEST_VARIANTS;

#undef X
}

TEST(serialize_sdp_line) {
    char buffer[20] = {0};

#define X(variant, expected)                                                                       \
    {                                                                                              \
        SmolRTSP_SdpLine_serialize(                                                                \
            (SmolRTSP_SdpLine){.ty = variant, .value = CharSlice99_from_str("abc")},               \
            smolrtsp_char_buffer_writer, buffer);                                                  \
        ASSERT_EQ(strcmp(buffer, expected "=abc\r\n"), 0);                                         \
        buffer[0] = '\0';                                                                          \
    }

    TEST_VARIANTS;

#undef X
}
