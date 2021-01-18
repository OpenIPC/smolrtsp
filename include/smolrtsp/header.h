/**
 * @file
 * An RTSP header.
 */

#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <smolrtsp/common.h>
#include <smolrtsp/user_writer.h>

#include <stdbool.h>
#include <stdio.h>

#include <slice99.h>

/**
 * An RTSP header.
 */
typedef struct {
    /**
     * The key of this header.
     */
    Slice99 key;

    /**
     * The value of this header.
     */
    Slice99 value;
} SmolRTSP_Header;

/**
 * `Accept`.
 */
#define SMOLRTSP_HEADER_NAME_ACCEPT (Slice99_from_str("Accept"))

/**
 * `Accept-Encoding`.
 */
#define SMOLRTSP_HEADER_NAME_ACCEPT_ENCODING (Slice99_from_str("Accept-Encoding"))

/**
 * `Accept-Language`.
 */
#define SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE (Slice99_from_str("Accept-Language"))

/**
 * `Allow`.
 */
#define SMOLRTSP_HEADER_NAME_ALLOW (Slice99_from_str("Allow"))

/**
 * `Authorization`.
 */
#define SMOLRTSP_HEADER_NAME_AUTHORIZATION (Slice99_from_str("Authorization"))

/**
 * `Bandwidth`.
 */
#define SMOLRTSP_HEADER_NAME_BANDWIDTH (Slice99_from_str("Bandwidth"))

/**
 * `Blocksize`.
 */
#define SMOLRTSP_HEADER_NAME_BLOCKSIZE (Slice99_from_str("Blocksize"))

/**
 * `Cache-Control`.
 */
#define SMOLRTSP_HEADER_NAME_CACHE_CONTROL (Slice99_from_str("Cache-Control"))

/**
 * `Conference`.
 */
#define SMOLRTSP_HEADER_NAME_CONFERENCE (Slice99_from_str("Conference"))

/**
 * `Connection`.
 */
#define SMOLRTSP_HEADER_NAME_CONNECTION (Slice99_from_str("Connection"))

/**
 * `Content-Base`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_BASE (Slice99_from_str("Content-Base"))

/**
 * `Content-Encoding`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_ENCODING (Slice99_from_str("Content-Encoding"))

/**
 * `Content-Language`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_LANGUAGE (Slice99_from_str("Content-Language"))

/**
 * `Content-Length`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_LENGTH (Slice99_from_str("Content-Length"))

/**
 * `Content-Location"`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_LOCATION (Slice99_from_str("Content-Location"))

/**
 * `Content-Type`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_TYPE (Slice99_from_str("Content-Type"))

/**
 * `CSeq`.
 */
#define SMOLRTSP_HEADER_NAME_C_SEQ (Slice99_from_str("CSeq"))

/**
 * `Date`.
 */
#define SMOLRTSP_HEADER_NAME_DATE (Slice99_from_str("Date"))

/**
 * `Expires`.
 */
#define SMOLRTSP_HEADER_NAME_EXPIRES (Slice99_from_str("Expires"))

/**
 * `From`.
 */
#define SMOLRTSP_HEADER_NAME_FROM (Slice99_from_str("From"))

/**
 * `If-Modified-Since`.
 */
#define SMOLRTSP_HEADER_NAME_IF_MODIFIED_SINCE (Slice99_from_str("If-Modified-Since"))

/**
 * `"Last-Modified`.
 */
#define SMOLRTSP_HEADER_NAME_LAST_MODIFIED (Slice99_from_str("Last-Modified"))

/**
 * `Proxy-Authenticate`.
 */
#define SMOLRTSP_HEADER_NAME_PROXY_AUTHENTICATE (Slice99_from_str("Proxy-Authenticate"))

/**
 * `Proxy-Require`.
 */
#define SMOLRTSP_HEADER_NAME_PROXY_REQUIRE (Slice99_from_str("Proxy-Require"))

/**
 * `Public`.
 */
#define SMOLRTSP_HEADER_NAME_PUBLIC (Slice99_from_str("Public"))

/**
 * `Range`.
 */
#define SMOLRTSP_HEADER_NAME_RANGE (Slice99_from_str("Range"))

/**
 * `Referrer`.
 */
#define SMOLRTSP_HEADER_NAME_REFERER (Slice99_from_str("Referrer"))

/**
 * `Require`.
 */
#define SMOLRTSP_HEADER_NAME_REQUIRE (Slice99_from_str("Require"))

/**
 * `Retry-After`.
 */
#define SMOLRTSP_HEADER_NAME_RETRY_AFTER (Slice99_from_str("Retry-After"))

/**
 * `RTP-Info`.
 */
#define SMOLRTSP_HEADER_NAME_RTP_INFO (Slice99_from_str("RTP-Info"))

/**
 * `Scale`.
 */
#define SMOLRTSP_HEADER_NAME_SCALE (Slice99_from_str("Scale"))

/**
 * `Session`.
 */
#define SMOLRTSP_HEADER_NAME_SESSION (Slice99_from_str("Session"))

/**
 * `Server`.
 */
#define SMOLRTSP_HEADER_NAME_SERVER (Slice99_from_str("Server"))

/**
 * `Speed`.
 */
#define SMOLRTSP_HEADER_NAME_SPEED (Slice99_from_str("Speed"))

/**
 * `Transport`.
 */
#define SMOLRTSP_HEADER_NAME_TRANSPORT (Slice99_from_str("Transport"))

/**
 * `Unsupported`.
 */
#define SMOLRTSP_HEADER_NAME_UNSUPPORTED (Slice99_from_str("Unsupported"))

/**
 * `User-Agent`.
 */
#define SMOLRTSP_HEADER_NAME_USER_AGENT (Slice99_from_str("User-Agent"))

/**
 * `Via`.
 */
#define SMOLRTSP_HEADER_NAME_VIA (Slice99_from_str("Via"))

/**
 * `WWW-Authenticate`.
 */
#define SMOLRTSP_HEADER_NAME_WWW_AUTHENTICATE (Slice99_from_str("WWW-Authenticate"))

/**
 * Serializes @p self to @p user_writer.
 *
 * @param[in] self The instance to be serialized.
 * @param[in] user_writer The function to be provided with serialized data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_Header_serialize(
    SmolRTSP_Header self, SmolRTSP_UserWriter user_writer, void *user_cx);

/**
 * Deserializes @p data into @p self.
 *
 * @pre `self != NULL`
 * @pre `data != NULL`
 */
SmolRTSP_DeserializeResult
SmolRTSP_Header_deserialize(SmolRTSP_Header *restrict self, Slice99 *restrict data);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_Header_eq(SmolRTSP_Header lhs, SmolRTSP_Header rhs);

/**
 * Pretty-prints @p self to @p stream.
 *
 * @pre `stream != NULL`
 */
void SmolRTSP_Header_dbg_to_file(SmolRTSP_Header self, FILE *stream);

/**
 * Pretty-prints @p self to `stdout`.
 */
void SmolRTSP_Header_dbg(SmolRTSP_Header self);

#endif // SMOLRTSP_HEADER_H
