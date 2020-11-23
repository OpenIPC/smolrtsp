#ifndef SMOLRTSP_DESERIALIZATION_H
#define SMOLRTSP_DESERIALIZATION_H

#define SMOLRTSP_CRLF "\r\n"

typedef enum {
    SmolRTSP_DeserializeResultOk,
    SmolRTSP_DeserializeResultErr,
    SmolRTSP_DeserializeResultNeedMore,
} SmolRTSP_DeserializeResult;

#endif // SMOLRTSP_DESERIALIZATION_H
