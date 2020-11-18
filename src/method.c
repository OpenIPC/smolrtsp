#include <method.h>

const char *
SmolRTSP_Method_serialize(SmolRTSP_Method method, SmolRTSP_UserWriter writer, void *user_cx) {
    writer(
        sizeof(smolrtsp_method_names[method]), (const void *)smolrtsp_method_names[method],
        user_cx);
}
