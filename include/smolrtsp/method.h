/**
 * @file
 *  An RTSP method.
 */

#ifndef SMOLRTSP_METHOD_H
#define SMOLRTSP_METHOD_H

#include <smolrtsp/common.h>

#include <slice99.h>

/**
 * An RTSP method.
 */
typedef Slice99 SmolRTSP_Method;

/**
 * `OPTIONS`.
 */
#define SMOLRTSP_METHOD_OPTIONS (Slice99_from_str("OPTIONS"))

/**
 * `DESCRIBE`.
 */
#define SMOLRTSP_METHOD_DESCRIBE (Slice99_from_str("DESCRIBE"))

/**
 * `ANNOUNCE`.
 */
#define SMOLRTSP_METHOD_ANNOUNCE (Slice99_from_str("ANNOUNCE"))

/**
 * `SETUP`.
 */
#define SMOLRTSP_METHOD_SETUP (Slice99_from_str("SETUP"))

/**
 * `PLAY`.
 */
#define SMOLRTSP_METHOD_PLAY (Slice99_from_str("PLAY"))

/**
 * `PAUSE`.
 */
#define SMOLRTSP_METHOD_PAUSE (Slice99_from_str("PAUSE"))

/**
 * `TEARDOWN`.
 */
#define SMOLRTSP_METHOD_TEARDOWN (Slice99_from_str("TEARDOWN"))

/**
 * `GET_PARAMETER`.
 */
#define SMOLRTSP_METHOD_GET_PARAMETER (Slice99_from_str("GET_PARAMETER"))

/**
 * `SET_PARAMETER`.
 */
#define SMOLRTSP_METHOD_SET_PARAMETER (Slice99_from_str("SET_PARAMETER"))

/**
 * `REDIRECT`.
 */
#define SMOLRTSP_METHOD_REDIRECT (Slice99_from_str("REDIRECT"))

/**
 * `RECORD`.
 */
#define SMOLRTSP_METHOD_RECORD (Slice99_from_str("RECORD"))

/**
 * Deserializes @p data into @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 */
SmolRTSP_DeserializeResult
SmolRTSP_Method_deserialize(SmolRTSP_Method *restrict self, Slice99 *restrict data);

#endif // SMOLRTSP_METHOD_H
