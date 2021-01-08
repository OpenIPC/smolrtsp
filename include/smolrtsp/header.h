/**
 * @file
 * An RTSP header.
 */

#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <smolrtsp/deserialization.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stdio.h>

#include <slice99.h>

/**
 * An RTSP header.
 */
typedef struct {
    Slice99 key;
    Slice99 value;
} SmolRTSP_Header;

#define SMOLRTSP_HEADER_NAME_ACCEPT             (Slice99_from_str("Accept"))
#define SMOLRTSP_HEADER_NAME_ACCEPT_ENCODING    (Slice99_from_str("Accept-Encoding"))
#define SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE    (Slice99_from_str("Accept-Language"))
#define SMOLRTSP_HEADER_NAME_ALLOW              (Slice99_from_str("Allow"))
#define SMOLRTSP_HEADER_NAME_AUTHORIZATION      (Slice99_from_str("Authorization"))
#define SMOLRTSP_HEADER_NAME_BANDWIDTH          (Slice99_from_str("Bandwidth"))
#define SMOLRTSP_HEADER_NAME_BLOCKSIZE          (Slice99_from_str("Blocksize"))
#define SMOLRTSP_HEADER_NAME_CACHE_CONTROL      (Slice99_from_str("Cache-Control"))
#define SMOLRTSP_HEADER_NAME_CONFERENCE         (Slice99_from_str("Conference"))
#define SMOLRTSP_HEADER_NAME_CONNECTION         (Slice99_from_str("Connection"))
#define SMOLRTSP_HEADER_NAME_CONTENT_BASE       (Slice99_from_str("Content-Base"))
#define SMOLRTSP_HEADER_NAME_CONTENT_ENCODING   (Slice99_from_str("Content-Encoding"))
#define SMOLRTSP_HEADER_NAME_CONTENT_LANGUAGE   (Slice99_from_str("Content-Language"))
#define SMOLRTSP_HEADER_NAME_CONTENT_LENGTH     (Slice99_from_str("Content-Length"))
#define SMOLRTSP_HEADER_NAME_CONTENT_LOCATION   (Slice99_from_str("Content-Location"))
#define SMOLRTSP_HEADER_NAME_CONTENT_TYPE       (Slice99_from_str("Content-Type"))
#define SMOLRTSP_HEADER_NAME_C_SEQ              (Slice99_from_str("CSeq"))
#define SMOLRTSP_HEADER_NAME_DATE               (Slice99_from_str("Date"))
#define SMOLRTSP_HEADER_NAME_EXPIRES            (Slice99_from_str("Expires"))
#define SMOLRTSP_HEADER_NAME_FROM               (Slice99_from_str("From"))
#define SMOLRTSP_HEADER_NAME_IF_MODIFIED_SINCE  (Slice99_from_str("If-Modified-Since"))
#define SMOLRTSP_HEADER_NAME_LAST_MODIFIED      (Slice99_from_str("Last-Modified"))
#define SMOLRTSP_HEADER_NAME_PROXY_AUTHENTICATE (Slice99_from_str("Proxy-Authenticate"))
#define SMOLRTSP_HEADER_NAME_PROXY_REQUIRE      (Slice99_from_str("Proxy-Require"))
#define SMOLRTSP_HEADER_NAME_PUBLIC             (Slice99_from_str("Public"))
#define SMOLRTSP_HEADER_NAME_RANGE              (Slice99_from_str("Range"))
#define SMOLRTSP_HEADER_NAME_REFERER            (Slice99_from_str("Referrer"))
#define SMOLRTSP_HEADER_NAME_REQUIRE            (Slice99_from_str("Require"))
#define SMOLRTSP_HEADER_NAME_RETRY_AFTER        (Slice99_from_str("Retry-After"))
#define SMOLRTSP_HEADER_NAME_RTP_INFO           (Slice99_from_str("RTP-Info"))
#define SMOLRTSP_HEADER_NAME_SCALE              (Slice99_from_str("Scale"))
#define SMOLRTSP_HEADER_NAME_SESSION            (Slice99_from_str("Session"))
#define SMOLRTSP_HEADER_NAME_SERVER             (Slice99_from_str("Server"))
#define SMOLRTSP_HEADER_NAME_SPEED              (Slice99_from_str("Speed"))
#define SMOLRTSP_HEADER_NAME_TRANSPORT          (Slice99_from_str("Transport"))
#define SMOLRTSP_HEADER_NAME_UNSUPPORTED        (Slice99_from_str("Unsupported"))
#define SMOLRTSP_HEADER_NAME_USER_AGENT         (Slice99_from_str("User-Agent"))
#define SMOLRTSP_HEADER_NAME_VIA                (Slice99_from_str("Via"))
#define SMOLRTSP_HEADER_NAME_WWW_AUTHENTICATE   (Slice99_from_str("WWW-Authenticate"))

/**
 * Serializes @p self to @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `self != NULL`
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

SmolRTSP_DeserializeResult SmolRTSP_Header_deserialize(
    SmolRTSP_Header *restrict self, Slice99 *restrict data, size_t *restrict bytes_read);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_Header_eq(const SmolRTSP_Header *restrict lhs, const SmolRTSP_Header *restrict rhs);

/**
 * Pretty-prints @p self to @p stream.
 *
 * @pre `self != NULL`
 * @pre `stream != NULL`
 */
void SmolRTSP_Header_dbg_to_file(const SmolRTSP_Header *self, FILE *stream);

/**
 * Pretty-prints @p self to `stdout`.
 *
 * @pre `self != NULL`
 */
void SmolRTSP_Header_dbg(const SmolRTSP_Header *self);

#endif // SMOLRTSP_HEADER_H
