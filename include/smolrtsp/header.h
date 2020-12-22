/**
 * @file
 * An RTSP header.
 */

#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <smolrtsp/slice.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stdio.h>

/**
 * An RTSP header.
 */
typedef struct {
    SmolRTSP_Slice key;
    SmolRTSP_Slice value;
} SmolRTSP_Header;

#define SMOLRTSP_HEADER_NAME_ACCEPT             (SmolRTSP_Slice_from_str("Accept"))
#define SMOLRTSP_HEADER_NAME_ACCEPT_ENCODING    (SmolRTSP_Slice_from_str("Accept-Encoding"))
#define SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE    (SmolRTSP_Slice_from_str("Accept-Language"))
#define SMOLRTSP_HEADER_NAME_ALLOW              (SmolRTSP_Slice_from_str("Allow"))
#define SMOLRTSP_HEADER_NAME_AUTHORIZATION      (SmolRTSP_Slice_from_str("Authorization"))
#define SMOLRTSP_HEADER_NAME_BANDWIDTH          (SmolRTSP_Slice_from_str("Bandwidth"))
#define SMOLRTSP_HEADER_NAME_BLOCKSIZE          (SmolRTSP_Slice_from_str("Blocksize"))
#define SMOLRTSP_HEADER_NAME_CACHE_CONTROL      (SmolRTSP_Slice_from_str("Cache-Control"))
#define SMOLRTSP_HEADER_NAME_CONFERENCE         (SmolRTSP_Slice_from_str("Conference"))
#define SMOLRTSP_HEADER_NAME_CONNECTION         (SmolRTSP_Slice_from_str("Connection"))
#define SMOLRTSP_HEADER_NAME_CONTENT_BASE       (SmolRTSP_Slice_from_str("Content-Base"))
#define SMOLRTSP_HEADER_NAME_CONTENT_ENCODING   (SmolRTSP_Slice_from_str("Content-Encoding"))
#define SMOLRTSP_HEADER_NAME_CONTENT_LANGUAGE   (SmolRTSP_Slice_from_str("Content-Language"))
#define SMOLRTSP_HEADER_NAME_CONTENT_LENGTH     (SmolRTSP_Slice_from_str("Content-Length"))
#define SMOLRTSP_HEADER_NAME_CONTENT_LOCATION   (SmolRTSP_Slice_from_str("Content-Location"))
#define SMOLRTSP_HEADER_NAME_CONTENT_TYPE       (SmolRTSP_Slice_from_str("Content-Type"))
#define SMOLRTSP_HEADER_NAME_C_SEQ              (SmolRTSP_Slice_from_str("CSeq"))
#define SMOLRTSP_HEADER_NAME_DATE               (SmolRTSP_Slice_from_str("Date"))
#define SMOLRTSP_HEADER_NAME_EXPIRES            (SmolRTSP_Slice_from_str("Expires"))
#define SMOLRTSP_HEADER_NAME_FROM               (SmolRTSP_Slice_from_str("From"))
#define SMOLRTSP_HEADER_NAME_IF_MODIFIED_SINCE  (SmolRTSP_Slice_from_str("If-Modified-Since"))
#define SMOLRTSP_HEADER_NAME_LAST_MODIFIED      (SmolRTSP_Slice_from_str("Last-Modified"))
#define SMOLRTSP_HEADER_NAME_PROXY_AUTHENTICATE (SmolRTSP_Slice_from_str("Proxy-Authenticate"))
#define SMOLRTSP_HEADER_NAME_PROXY_REQUIRE      (SmolRTSP_Slice_from_str("Proxy-Require"))
#define SMOLRTSP_HEADER_NAME_PUBLIC             (SmolRTSP_Slice_from_str("Public"))
#define SMOLRTSP_HEADER_NAME_RANGE              (SmolRTSP_Slice_from_str("Range"))
#define SMOLRTSP_HEADER_NAME_REFERER            (SmolRTSP_Slice_from_str("Referrer"))
#define SMOLRTSP_HEADER_NAME_REQUIRE            (SmolRTSP_Slice_from_str("Require"))
#define SMOLRTSP_HEADER_NAME_RETRY_AFTER        (SmolRTSP_Slice_from_str("Retry-After"))
#define SMOLRTSP_HEADER_NAME_RTP_INFO           (SmolRTSP_Slice_from_str("RTP-Info"))
#define SMOLRTSP_HEADER_NAME_SCALE              (SmolRTSP_Slice_from_str("Scale"))
#define SMOLRTSP_HEADER_NAME_SESSION            (SmolRTSP_Slice_from_str("Session"))
#define SMOLRTSP_HEADER_NAME_SERVER             (SmolRTSP_Slice_from_str("Server"))
#define SMOLRTSP_HEADER_NAME_SPEED              (SmolRTSP_Slice_from_str("Speed"))
#define SMOLRTSP_HEADER_NAME_TRANSPORT          (SmolRTSP_Slice_from_str("Transport"))
#define SMOLRTSP_HEADER_NAME_UNSUPPORTED        (SmolRTSP_Slice_from_str("Unsupported"))
#define SMOLRTSP_HEADER_NAME_USER_AGENT         (SmolRTSP_Slice_from_str("User-Agent"))
#define SMOLRTSP_HEADER_NAME_VIA                (SmolRTSP_Slice_from_str("Via"))
#define SMOLRTSP_HEADER_NAME_WWW_AUTHENTICATE   (SmolRTSP_Slice_from_str("WWW-Authenticate"))

/**
 * Serializes @p self into @p user_writer.
 *
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre @p self shall not be `NULL`.
 * @pre @p user_writer shall not be `NULL`.
 */
void SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Compares @p lhs with @p rhs for equality.
 *
 * @return `true` if @p lhs and @rhs are equal, `false` otherwise.
 *
 * @pre @p lhs shall not be `NULL`.
 * @pre @p rhs shall not be `NULL`.
 */
bool SmolRTSP_Header_eq(const SmolRTSP_Header *restrict lhs, const SmolRTSP_Header *restrict rhs);

/**
 * Pretty-prints @p self to @p stream.
 *
 * @pre @p self shall not be `NULL`.
 * @pre `self.key` and `self.value` shall not be null-slices.
 * @pre @p stream shall not be `NULL`.
 */
void SmolRTSP_Header_dbg_to_file(const SmolRTSP_Header *self, FILE *stream);

/**
 * Pretty-prints @p self to `stdout`.
 *
 * @pre @p self shall not be `NULL`.
 * @pre `self.key` and `self.value` shall not be null-slices.
 */
void SmolRTSP_Header_dbg(const SmolRTSP_Header *self);

#endif // SMOLRTSP_HEADER_H
