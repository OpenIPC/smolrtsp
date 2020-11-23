#ifndef SMOLRTSP_DESERIALIZATION_H
#define SMOLRTSP_DESERIALIZATION_H

#include <stddef.h>

typedef enum {
    SmolRTSP_DeserializeResultOk,
    SmolRTSP_DeserializeResultErr,
    SmolRTSP_DeserializeResultNeedMore,
} SmolRTSP_DeserializeResult;

#endif // SMOLRTSP_DESERIALIZATION_H
