#ifndef SMOLRTSP_REQUEST_URI_H
#define SMOLRTSP_REQUEST_URI_H

#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

typedef char SmolRTSP_RequestURI[SMOLRTSP_REQUEST_URI_SIZE];

void SmolRTSP_RequestURI_serialize(
    const SmolRTSP_RequestURI *restrict uri, SmolRTSP_UserWriter user_writer, void *user_cx);

#endif // SMOLRTSP_REQUEST_URI_H
