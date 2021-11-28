#include <smolrtsp/sdp.h>

#include "parsing.h"

#include <assert.h>
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

void SmolRTSP_SdpLine_serialize(
    SmolRTSP_SdpLine self, SmolRTSP_UserWriter user_writer, void *user_cx) {

    const char *ty = SmolRTSP_SdpLineType_str(self.ty);

    user_writer(CharSlice99_from_str((char *)ty), user_cx);
    user_writer(CharSlice99_from_str("="), user_cx);
    user_writer(self.value, user_cx);
    user_writer(SMOLRTSP_CRLF, user_cx);
}
