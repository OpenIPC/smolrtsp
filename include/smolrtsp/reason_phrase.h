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

/**
 * Deserializes @p data into @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 */
SmolRTSP_DeserializeResult
SmolRTSP_ReasonPhrase_deserialize(SmolRTSP_ReasonPhrase *restrict self, Slice99 *restrict data);

#endif // SMOLRTSP_REASON_PHRASE_H
