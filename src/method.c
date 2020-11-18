#include <method.h>

const char *smol_rtsp_method_stringify(SmolRTSP_Method method) {
#define ASSOC(tag, string)                                                                         \
    case tag:                                                                                      \
        return string

    switch (method) {
        ASSOC(SmolRTSP_MethodDescribe, "DESCRIBE");
        ASSOC(SmolRTSP_MethodGetParameter, "GET_PARAMETER");
        ASSOC(SmolRTSP_MethodOptions, "OPTIONS");
        ASSOC(SmolRTSP_MethodPause, "PAUSE");
        ASSOC(SmolRTSP_MethodPlay, "PLAY");
        ASSOC(SmolRTSP_MethodPlayNotify, "PLAY_NOTIFY");
        ASSOC(SmolRTSP_MethodRedirect, "REDIRECT");
        ASSOC(SmolRTSP_MethodSetup, "SETUP");
        ASSOC(SmolRTSP_MethodSetParameter, "SET_PARAMETER");
        ASSOC(SmolRTSP_MethodTeardown, "TEARDOWN");
    }

#undef METHOD
}
