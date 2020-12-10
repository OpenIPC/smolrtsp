/**
 * @file
 * @brief An RTSP reason phrase.
 */

#ifndef SMOLRTSP_REASON_PHRASE_H
#define SMOLRTSP_REASON_PHRASE_H

#include <smolrtsp/limits.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>

/**
 * An RTSP reason phrase.
 */
typedef struct {
    char data[SMOLRTSP_REASON_PHRASE_SIZE];
} SmolRTSP_ReasonPhrase;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 */
void SmolRTSP_ReasonPhrase_serialize(
    const SmolRTSP_ReasonPhrase *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Compares @p lhs with @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre @p lhs shall not be `NULL`.
 * @pre @p rhs shall not be `NULL`.
 */
bool SmolRTSP_ReasonPhrase_eq(const SmolRTSP_ReasonPhrase *lhs, const SmolRTSP_ReasonPhrase *rhs);

#endif // SMOLRTSP_REASON_PHRASE_H
