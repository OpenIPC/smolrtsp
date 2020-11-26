#ifndef SMOLRTSP_REQUEST_URI_H
#define SMOLRTSP_REQUEST_URI_H

#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

typedef struct {
    char data[SMOLRTSP_REQUEST_URI_SIZE];
} SmolRTSP_RequestURI;

void SmolRTSP_RequestURI_serialize(
    const SmolRTSP_RequestURI *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

#endif // SMOLRTSP_REQUEST_URI_H
