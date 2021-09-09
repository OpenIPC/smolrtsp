#include <smolrtsp/sdp.h>

#include "correctness.h"
#include "parsing.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

const char *SmolRTSP_SdpLineType_str(SmolRTSP_SdpLineType self) {
    switch (self) {
    case SmolRTSP_SdpLineType_Version: return "v";
    case SmolRTSP_SdpLineType_Origin: return "o";
    case SmolRTSP_SdpLineType_SessionName: return "s";
    case SmolRTSP_SdpLineType_Connection: return "c";
    case SmolRTSP_SdpLineType_Media: return "m";
    case SmolRTSP_SdpLineType_Attr: return "a";
    case SmolRTSP_SdpLineType_Time: return "t";
    default: return NULL;
    }
}

void SmolRTSP_SdpLineType_serialize(
    SmolRTSP_SdpLineType self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    user_writer(CharSlice99_from_str((char *)SmolRTSP_SdpLineType_str(self)), user_cx);
}
