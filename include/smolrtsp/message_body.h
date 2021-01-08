/**
 * @file
 *  An RTSP message body.
 */

#ifndef SMOLRTSP_MESSAGE_BODY_H
#define SMOLRTSP_MESSAGE_BODY_H

#include <smolrtsp/deserialization.h>

#include <slice99.h>

/**
 * An RTSP message body.
 */
typedef Slice99 SmolRTSP_MessageBody;

SmolRTSP_DeserializeResult SmolRTSP_MessageBody_deserialize(
    SmolRTSP_MessageBody *restrict self, Slice99 *restrict data, size_t content_length);

#endif // SMOLRTSP_MESSAGE_BODY_H
