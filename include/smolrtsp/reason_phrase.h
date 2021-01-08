/**
 * @file
 *  An RTSP reason phrase.
 */

#ifndef SMOLRTSP_REASON_PHRASE_H
#define SMOLRTSP_REASON_PHRASE_H

#include <smolrtsp/deserialization.h>

#include <slice99.h>

/**
 * An RTSP reason phrase.
 */
typedef Slice99 SmolRTSP_ReasonPhrase;

SmolRTSP_DeserializeResult SmolRTSP_ReasonPhrase_deserialize(
    SmolRTSP_ReasonPhrase *restrict self, Slice99 *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_REASON_PHRASE_H
