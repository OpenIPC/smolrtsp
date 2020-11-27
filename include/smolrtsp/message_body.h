#ifndef SMOLRTSP_MESSAGE_BODY_H
#define SMOLRTSP_MESSAGE_BODY_H

#include <smolrtsp/user_writer.h>

#include <stddef.h>

typedef struct {
    size_t size;
    const void *data;
} SmolRTSP_MessageBody;

SmolRTSP_MessageBody SmolRTSP_MessageBody_empty(void);

void SmolRTSP_MessageBody_serialize(
    const SmolRTSP_MessageBody *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

#endif // SMOLRTSP_MESSAGE_BODY_H
