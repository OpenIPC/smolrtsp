/**
 * @file
 * An RTSP response line.
 */

#ifndef SMOLRTSP_RESPONSE_LINE_H
#define SMOLRTSP_RESPONSE_LINE_H

#include <smolrtsp/reason_phrase.h>
#include <smolrtsp/rtsp_version.h>
#include <smolrtsp/status_code.h>

/**
 * An RTSP response line.
 */
typedef struct {
    /**
     * The RTSP version used.
     */
    SmolRTSP_RTSPVersion version;

    /**
     * The status code.
     */
    SmolRTSP_StatusCode code;

    /**
     * The reason phrase.
     */
    SmolRTSP_ReasonPhrase reason;
} SmolRTSP_ResponseLine;

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_ResponseLine_serialize(
    SmolRTSP_ResponseLine self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state of deserialization of #SmolRTSP_RequestLine.
 */
typedef enum {
    SmolRTSP_ResponseLineDeserializerStateRTSPVersion,
    SmolRTSP_ResponseLineDeserializerStateStatusCode,
    SmolRTSP_ResponseLineDeserializerStateReasonPhrase,
    SmolRTSP_ResponseLineDeserializerStateDone,
} SmolRTSP_ResponseLineDeserializerState;

SmolRTSP_DeserializeResult SmolRTSP_ResponseLine_deserialize(
    SmolRTSP_ResponseLine *restrict self, Slice99 *restrict data,
    SmolRTSP_ResponseLineDeserializerState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 */
bool SmolRTSP_ResponseLine_eq(SmolRTSP_ResponseLine lhs, SmolRTSP_ResponseLine rhs);

#endif // SMOLRTSP_RESPONSE_LINE_H
