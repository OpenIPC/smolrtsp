/**
 * @file
 * @brief An RTSP request URI.
 */

#ifndef SMOLRTSP_REQUEST_URI_H
#define SMOLRTSP_REQUEST_URI_H

#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>

typedef struct {
    char data[SMOLRTSP_REQUEST_URI_SIZE];
} SmolRTSP_RequestURI;

void SmolRTSP_RequestURI_serialize(
    const SmolRTSP_RequestURI *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

bool SmolRTSP_RequestURI_eq(const SmolRTSP_RequestURI *lhs, const SmolRTSP_RequestURI *rhs);

#endif // SMOLRTSP_REQUEST_URI_H
