#ifndef SMOLRTSP_METHOD_H
#define SMOLRTSP_METHOD_H

typedef enum {
    SmolRTSP_MethodDescribe,
    SmolRTSP_MethodGetParameter,
    SmolRTSP_MethodOptions,
    SmolRTSP_MethodPause,
    SmolRTSP_MethodPlay,
    SmolRTSP_MethodPlayNotify,
    SmolRTSP_MethodRedirect,
    SmolRTSP_MethodSetup,
    SmolRTSP_MethodSetParameter,
    SmolRTSP_MethodTeardown,
} SmolRTSP_Method;

const char *smolrtsp_method_stringify(SmolRTSP_Method method);

#endif // SMOLRTSP_METHOD_H
