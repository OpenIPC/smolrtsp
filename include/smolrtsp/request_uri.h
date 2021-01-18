/**
 * @file
 *  An RTSP request URI.
 */

#ifndef SMOLRTSP_REQUEST_URI_H
#define SMOLRTSP_REQUEST_URI_H

#include <smolrtsp/common.h>

#include <slice99.h>

/**
 * An RTSP request URI.
 */
typedef Slice99 SmolRTSP_RequestURI;

/**
 * Deserializes @p data into @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 */
SmolRTSP_DeserializeResult
SmolRTSP_RequestURI_deserialize(SmolRTSP_RequestURI *restrict self, Slice99 *restrict data);

#endif // SMOLRTSP_REQUEST_URI_H
