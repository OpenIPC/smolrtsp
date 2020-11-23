#ifndef SMOLRTSP_METHOD_H
#define SMOLRTSP_METHOD_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

typedef char SmolRTSP_Method[SMOLRTSP_METHOD_SIZE];

#define SMOLRTSP_METHOD_DESCRIBE      "DESCRIBE"
#define SMOLRTSP_METHOD_GET_PARAMETER "GET_PARAMETER"
#define SMOLRTSP_METHOD_OPTIONS       "OPTIONS"
#define SMOLRTSP_METHOD_PAUSE         "PAUSE"
#define SMOLRTSP_METHOD_PLAY          "PLAY"
#define SMOLRTSP_METHOD_PLAY_NOTIFY   "PLAY_NOTIFY"
#define SMOLRTSP_METHOD_REDIRECT      "REDIRECT"
#define SMOLRTSP_METHOD_SETUP         "SETUP"
#define SMOLRTSP_METHOD_SET_PARAMETER "SET_PARAMETER"
#define SMOLRTSP_METHOD_TEARDOWN      "TEARDOWN"

void SmolRTSP_Method_serialize(
    const SmolRTSP_Method method, SmolRTSP_UserWriter user_writer, void *user_cx);
SmolRTSP_DeserializeResult SmolRTSP_Method_deserialize(
    SmolRTSP_Method *restrict method, size_t size, const void *restrict data,
    size_t *restrict bytes_read);

#endif // SMOLRTSP_METHOD_H
