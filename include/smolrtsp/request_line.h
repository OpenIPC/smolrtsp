
/**
 * @file
 * An RTSP request line.
 */

#ifndef SMOLRTSP_REQUEST_LINE_H
#define SMOLRTSP_REQUEST_LINE_H

#include <smolrtsp/error.h>

#include <smolrtsp/method.h>
#include <smolrtsp/request_uri.h>
#include <smolrtsp/rtsp_version.h>

#include <stdbool.h>

/**
 * The start state of #SmolRTSP_RequestLineParseState.
 */
#define SMOLRTSP_REQUEST_LINE_PARSE_STATE_INIT                                                     \
    (SmolRTSP_RequestLineParseState) {                                                             \
        .tag = SmolRTSP_RequestLineParseState_Method,                                              \
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
    SmolRTSP_RtspVersion version;
} SmolRTSP_RequestLine;

/**
 * Serialises @p self to @p user_writer.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] user_writer The function to be provided with serialised data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_RequestLine_serialize(
    SmolRTSP_RequestLine self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * A state of parsing of #SmolRTSP_RequestLine.
 */
typedef struct {
    /**
     * What part of a request line is being parsed right now.
     */
    enum {
        SmolRTSP_RequestLineParseState_Method,
        SmolRTSP_RequestLineParseState_RequestURI,
        SmolRTSP_RequestLineParseState_RtspVersion,
        SmolRTSP_RequestLineParseState_Crlf,
        SmolRTSP_RequestLineParseState_Done,
    } tag;
} SmolRTSP_RequestLineParseState;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `state != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_RequestLine_parse(
    SmolRTSP_RequestLine *restrict self, CharSlice99 input,
    SmolRTSP_RequestLineParseState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_RequestLine_eq(SmolRTSP_RequestLine lhs, SmolRTSP_RequestLine rhs);

#endif // SMOLRTSP_REQUEST_LINE_H
