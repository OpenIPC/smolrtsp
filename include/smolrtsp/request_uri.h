/**
 * @file
 * An RTSP request URI.
 */

#ifndef SMOLRTSP_REQUEST_URI_H
#define SMOLRTSP_REQUEST_URI_H

#include <smolrtsp/error.h>

#include <slice99.h>

/**
 * An RTSP request URI.
 */
typedef CharSlice99 SmolRTSP_RequestURI;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult
SmolRTSP_RequestURI_parse(SmolRTSP_RequestURI *restrict self, CharSlice99 input);

#endif // SMOLRTSP_REQUEST_URI_H
