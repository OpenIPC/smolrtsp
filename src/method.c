#include <method.h>

const char *smolrtsp_method_stringify(SmolRTSP_Method method) {
#define ASSOC(tag, string)                                                                         \
    case SmolRTSP_Method##tag:                                                                     \
        return string

    switch (method) {
        ASSOC(Describe, "DESCRIBE");
        ASSOC(GetParameter, "GET_PARAMETER");
        ASSOC(Options, "OPTIONS");
        ASSOC(Pause, "PAUSE");
        ASSOC(Play, "PLAY");
        ASSOC(PlayNotify, "PLAY_NOTIFY");
        ASSOC(Redirect, "REDIRECT");
        ASSOC(Setup, "SETUP");
        ASSOC(SetParameter, "SET_PARAMETER");
        ASSOC(Teardown, "TEARDOWN");
    }

#undef METHOD
}
