
/**
 * @file
 *  An RTSP request line.
 */

#ifndef SMOLRTSP_REQUEST_LINE_H
#define SMOLRTSP_REQUEST_LINE_H

#include <smolrtsp/method.h>
#include <smolrtsp/request_uri.h>
#include <smolrtsp/rtsp_version.h>

#include <stdbool.h>

/**
 * The start state of #SmolRTSP_RequestLineDeserializerState.
 */
#define SMOLRTSP_REQUEST_LINE_DESERIALIZER_STATE_INIT                                              \
    (SmolRTSP_RequestLineDeserializerState) {                                                      \
        .tag = SmolRTSP_RequestLineDeserializerStateMethod,                                        \
    }

/**
 * An RTSP request line.
 */
typedef struct {
    /**
     * The method used.
     */
    SmolRTSP_Method method;

    /**
     * The request URI.
     */
    SmolRTSP_RequestURI uri;

    /**
     * The RTSP version used.
     */
    SmolRTSP_RTSPVersion version;
} SmolRTSP_RequestLine;

/**
 * Serializes @p self to @p user_writer.
 *
 * @param[in] self The instance to be serialized.
 * @param[in] user_writer The function to be provided with serialized data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_RequestLine_serialize(
    SmolRTSP_RequestLine self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state of deserialization of #SmolRTSP_RequestLine.
 */
typedef struct {
    /**
     * What part of a request line is being deserialized right now.
     */
    enum {
        SmolRTSP_RequestLineDeserializerStateMethod,
        SmolRTSP_RequestLineDeserializerStateRequestURI,
        SmolRTSP_RequestLineDeserializerStateRTSPVersion,
        SmolRTSP_RequestLineDeserializerStateCRLF,
        SmolRTSP_RequestLineDeserializerStateDone,
    } tag;
} SmolRTSP_RequestLineDeserializerState;

/**
 * Deserializes @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 * @pre `state != NULL`
 */
SmolRTSP_DeserializeResult SmolRTSP_RequestLine_deserialize(
    SmolRTSP_RequestLine *restrict self, Slice99 *restrict data,
    SmolRTSP_RequestLineDeserializerState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_RequestLine_eq(SmolRTSP_RequestLine lhs, SmolRTSP_RequestLine rhs);

#endif // SMOLRTSP_REQUEST_LINE_H
