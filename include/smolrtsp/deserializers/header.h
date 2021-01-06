/**
 * @file
 *  An RTSP header deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_HEADER_H
#define SMOLRTSP_DESERIALIZERS_HEADER_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/header.h>
#include <smolrtsp/opaque_type.h>

/**
 * A deserializer of an RTSP header.
 */
SMOLRTSP_OPAQUE_TYPE(SmolRTSP_HeaderDeserializer);

SmolRTSP_HeaderDeserializer *SmolRTSP_HeaderDeserializer_new(void);
void SmolRTSP_HeaderDeserializer_free(SmolRTSP_HeaderDeserializer *self);

size_t SmolRTSP_HeaderDeserializer_bytes_read(SmolRTSP_HeaderDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_HeaderDeserializer_deserialize(
    SmolRTSP_HeaderDeserializer *restrict self, SmolRTSP_Header *restrict result,
    Slice99 *restrict data);

#endif // SMOLRTSP_DESERIALIZERS_HEADER_H
