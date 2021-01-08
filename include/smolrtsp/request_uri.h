/**
 * @file
 *  An RTSP request URI.
 */

#ifndef SMOLRTSP_REQUEST_URI_H
#define SMOLRTSP_REQUEST_URI_H

#include <smolrtsp/deserialization.h>

#include <slice99.h>

/**
 * An RTSP request URI.
 */
typedef Slice99 SmolRTSP_RequestURI;

SmolRTSP_DeserializeResult SmolRTSP_RequestURI_deserialize(
    SmolRTSP_RequestURI *restrict self, Slice99 *restrict data, size_t *restrict bytes_read);

#endif // SMOLRTSP_REQUEST_URI_H
