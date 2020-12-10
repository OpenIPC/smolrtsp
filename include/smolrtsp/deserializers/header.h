/**
 * @file
 * @brief An RTSP header deserializer.
 */

#ifndef SMOLRTSP_DESERIALIZERS_HEADER_H
#define SMOLRTSP_DESERIALIZERS_HEADER_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/header.h>

struct SmolRTSP_HeaderDeserializer;
typedef struct SmolRTSP_HeaderDeserializer SmolRTSP_HeaderDeserializer;

SmolRTSP_HeaderDeserializer *SmolRTSP_HeaderDeserializer_new(void);
void SmolRTSP_HeaderDeserializer_free(SmolRTSP_HeaderDeserializer *self);

SmolRTSP_Header SmolRTSP_HeaderDeserializer_inner(SmolRTSP_HeaderDeserializer *self);
size_t SmolRTSP_HeaderDeserializer_bytes_read(SmolRTSP_HeaderDeserializer *self);

SmolRTSP_DeserializeResult SmolRTSP_HeaderDeserializer_deserialize(
    SmolRTSP_HeaderDeserializer *restrict self, size_t size, const char data[restrict]);

#endif // SMOLRTSP_DESERIALIZERS_HEADER_H
