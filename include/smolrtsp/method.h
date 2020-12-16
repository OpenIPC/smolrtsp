/**
 * @file
 * @brief An RTSP method.
 */

#ifndef SMOLRTSP_METHOD_H
#define SMOLRTSP_METHOD_H

#include <smolrtsp/slice.h>

/**
 * An RTSP method.
 */
typedef SmolRTSP_Slice SmolRTSP_Method;

#define SMOLRTSP_METHOD_OPTIONS       (SmolRTSP_Slice_from_str("OPTIONS"))
#define SMOLRTSP_METHOD_DESCRIBE      (SmolRTSP_Slice_from_str("DESCRIBE"))
#define SMOLRTSP_METHOD_ANNOUNCE      (SmolRTSP_Slice_from_str("ANNOUNCE"))
#define SMOLRTSP_METHOD_SETUP         (SmolRTSP_Slice_from_str("SETUP"))
#define SMOLRTSP_METHOD_PLAY          (SmolRTSP_Slice_from_str("PLAY"))
#define SMOLRTSP_METHOD_PAUSE         (SmolRTSP_Slice_from_str("PAUSE"))
#define SMOLRTSP_METHOD_TEARDOWN      (SmolRTSP_Slice_from_str("TEARDOWN"))
#define SMOLRTSP_METHOD_GET_PARAMETER (SmolRTSP_Slice_from_str("GET_PARAMETER"))
#define SMOLRTSP_METHOD_SET_PARAMETER (SmolRTSP_Slice_from_str("SET_PARAMETER"))
#define SMOLRTSP_METHOD_REDIRECT      (SmolRTSP_Slice_from_str("REDIRECT"))
#define SMOLRTSP_METHOD_RECORD        (SmolRTSP_Slice_from_str("RECORD"))

#endif // SMOLRTSP_METHOD_H
