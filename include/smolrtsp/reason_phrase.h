/**
 * @file
 * @brief An RTSP reason phrase.
 */

#ifndef SMOLRTSP_REASON_PHRASE_H
#define SMOLRTSP_REASON_PHRASE_H

#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>

typedef struct {
    char data[SMOLRTSP_REASON_PHRASE_SIZE];
} SmolRTSP_ReasonPhrase;

void SmolRTSP_ReasonPhrase_serialize(
    const SmolRTSP_ReasonPhrase *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

bool SmolRTSP_ReasonPhrase_eq(const SmolRTSP_ReasonPhrase *lhs, const SmolRTSP_ReasonPhrase *rhs);

#endif // SMOLRTSP_REASON_PHRASE_H
