/**
 * @file
 *  An RTSP message body.
 */

#ifndef SMOLRTSP_MESSAGE_BODY_H
#define SMOLRTSP_MESSAGE_BODY_H

#include <smolrtsp/common.h>

#include <slice99.h>

/**
 * An RTSP message body.
 */
typedef Slice99 SmolRTSP_MessageBody;

/**
 * Deserializes @p data into @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 */
SmolRTSP_DeserializeResult SmolRTSP_MessageBody_deserialize(
    SmolRTSP_MessageBody *restrict self, Slice99 *restrict data, size_t content_length);

#endif // SMOLRTSP_MESSAGE_BODY_H
