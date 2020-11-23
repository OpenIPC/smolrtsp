#ifndef SMOLRTSP_RTSP_VERSION_H
#define SMOLRTSP_RTSP_VERSION_H

#include <smolrtsp/user_writer.h>

#include <stdint.h>

typedef struct {
    uint_least8_t major;
    uint_least8_t minor;
} SmolRTSP_RTSPVersion;

void SmolRTSP_RTSPVersion_serialize(
    const SmolRTSP_RTSPVersion version, SmolRTSP_UserWriter user_writer, void *user_cx);

SmolRTSP_DeserializeResult SmolRTSP_RTSPVersion_deserialize(
    SmolRTSP_RTSPVersion *restrict version, size_t size, const void *restrict data);

#endif // SMOLRTSP_RTSP_VERSION_H
