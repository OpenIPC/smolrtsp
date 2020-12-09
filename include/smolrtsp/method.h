/**
 * @file
 * @brief An RTSP method.
 */

#ifndef SMOLRTSP_METHOD_H
#define SMOLRTSP_METHOD_H

#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>

typedef struct {
    char data[SMOLRTSP_METHOD_SIZE];
} SmolRTSP_Method;

#define SMOLRTSP_METHOD_OPTIONS       ((SmolRTSP_Method){"OPTIONS"})
#define SMOLRTSP_METHOD_DESCRIBE      ((SmolRTSP_Method){"DESCRIBE"})
#define SMOLRTSP_METHOD_ANNOUNCE      ((SmolRTSP_Method){"ANNOUNCE"})
#define SMOLRTSP_METHOD_SETUP         ((SmolRTSP_Method){"SETUP"})
#define SMOLRTSP_METHOD_PLAY          ((SmolRTSP_Method){"PLAY"})
#define SMOLRTSP_METHOD_PAUSE         ((SmolRTSP_Method){"PAUSE"})
#define SMOLRTSP_METHOD_TEARDOWN      ((SmolRTSP_Method){"TEARDOWN"})
#define SMOLRTSP_METHOD_GET_PARAMETER ((SmolRTSP_Method){"GET_PARAMETER"})
#define SMOLRTSP_METHOD_SET_PARAMETER ((SmolRTSP_Method){"SET_PARAMETER"})
#define SMOLRTSP_METHOD_REDIRECT      ((SmolRTSP_Method){"REDIRECT"})
#define SMOLRTSP_METHOD_RECORD        ((SmolRTSP_Method){"RECORD"})

void SmolRTSP_Method_serialize(
    const SmolRTSP_Method *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

bool SmolRTSP_Method_eq(const SmolRTSP_Method *lhs, const SmolRTSP_Method *rhs);

#endif // SMOLRTSP_METHOD_H
