/**
 * @file
 * @brief An RTSP message body.
 */

#ifndef SMOLRTSP_MESSAGE_BODY_H
#define SMOLRTSP_MESSAGE_BODY_H

#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const char *data;
    size_t size;
} SmolRTSP_MessageBody;

SmolRTSP_MessageBody SmolRTSP_MessageBody_empty(void);

void SmolRTSP_MessageBody_serialize(
    const SmolRTSP_MessageBody *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

bool SmolRTSP_MessageBody_eq(const SmolRTSP_MessageBody *lhs, const SmolRTSP_MessageBody *rhs);

#endif // SMOLRTSP_MESSAGE_BODY_H
