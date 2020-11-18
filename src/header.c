#include <header.h>

void SmolRTSP_KnownHeader_serialize(
    SmolRTSP_KnownHeader header, SmolRTSP_UserWriter writer, void *user_cx) {
    writer(
        sizeof(smolrtsp_header_names[header]), (const void *)smolrtsp_header_names[header],
        user_cx);
}
