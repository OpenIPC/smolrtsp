/**
 * @file
 * @brief An RTSP version.
 */

#ifndef SMOLRTSP_RTSP_VERSION_H
#define SMOLRTSP_RTSP_VERSION_H

#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint_least8_t major;
    uint_least8_t minor;
} SmolRTSP_RTSPVersion;

void SmolRTSP_RTSPVersion_serialize(
    const SmolRTSP_RTSPVersion *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

bool SmolRTSP_RTSPVersion_eq(const SmolRTSP_RTSPVersion *lhs, const SmolRTSP_RTSPVersion *rhs);

#endif // SMOLRTSP_RTSP_VERSION_H
