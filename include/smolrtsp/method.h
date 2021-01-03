/**
 * @file
 *  An RTSP method.
 */

#ifndef SMOLRTSP_METHOD_H
#define SMOLRTSP_METHOD_H

#include <slice99.h>

/**
 * An RTSP method.
 */
typedef Slice99 SmolRTSP_Method;

#define SMOLRTSP_METHOD_OPTIONS       (Slice99_from_str("OPTIONS"))
#define SMOLRTSP_METHOD_DESCRIBE      (Slice99_from_str("DESCRIBE"))
#define SMOLRTSP_METHOD_ANNOUNCE      (Slice99_from_str("ANNOUNCE"))
#define SMOLRTSP_METHOD_SETUP         (Slice99_from_str("SETUP"))
#define SMOLRTSP_METHOD_PLAY          (Slice99_from_str("PLAY"))
#define SMOLRTSP_METHOD_PAUSE         (Slice99_from_str("PAUSE"))
#define SMOLRTSP_METHOD_TEARDOWN      (Slice99_from_str("TEARDOWN"))
#define SMOLRTSP_METHOD_GET_PARAMETER (Slice99_from_str("GET_PARAMETER"))
#define SMOLRTSP_METHOD_SET_PARAMETER (Slice99_from_str("SET_PARAMETER"))
#define SMOLRTSP_METHOD_REDIRECT      (Slice99_from_str("REDIRECT"))
#define SMOLRTSP_METHOD_RECORD        (Slice99_from_str("RECORD"))

#endif // SMOLRTSP_METHOD_H
