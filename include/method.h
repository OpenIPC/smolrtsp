#ifndef SMOLRTSP_METHOD_H
#define SMOLRTSP_METHOD_H

#include <user_writer.h>

#define DEF_METHODS()                                                                              \
    X(Describe, "DESCRIBE")                                                                        \
    X(GetParameter, "GET_PARAMETER")                                                               \
    X(Options, "OPTIONS")                                                                          \
    X(Pause, "PAUSE")                                                                              \
    X(Play, "PLAY")                                                                                \
    X(PlayNotify, "PLAY_NOTIFY")                                                                   \
    X(Redirect, "REDIRECT")                                                                        \
    X(Setup, "SETUP")                                                                              \
    X(SetParameter, "SET_PARAMETER")                                                               \
    X(Teardown, "TEARDOWN")

#define X(method, _stringification) SmolRTSP_Method##method,
typedef enum { DEF_METHODS() } SmolRTSP_Method;
#undef X

#define X(method, stringification) [SmolRTSP_Method##method] = stringification,
static const char smolrtsp_method_names[][10] = {DEF_METHODS()};
#undef X

#undef DEF_METHODS

const char *
SmolRTSP_Method_serialize(SmolRTSP_Method method, SmolRTSP_UserWriter writer, void *user_cx);

#endif // SMOLRTSP_METHOD_H
