/**
 * @file
 * An RTSP header.
 */

#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <smolrtsp/types/error.h>
#include <smolrtsp/writer.h>

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
    CharSlice99 key;

    /**
     * The value of this header.
     */
    CharSlice99 value;
} SmolRTSP_Header;

/**
 * Serialises @p self into @p w.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The writer to be provided with serialised data.
 *
 * @pre `w.self && w.vptr`
 */
void SmolRTSP_Header_serialize(SmolRTSP_Header self, SmolRTSP_Writer w);

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_Header_parse(SmolRTSP_Header *restrict self, CharSlice99 input);

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

/**
 * `Accept`.
 */
#define SMOLRTSP_HEADER_NAME_ACCEPT (CharSlice99_from_str("Accept"))

/**
 * `Accept-Encoding`.
 */
#define SMOLRTSP_HEADER_NAME_ACCEPT_ENCODING (CharSlice99_from_str("Accept-Encoding"))

/**
 * `Accept-Language`.
 */
#define SMOLRTSP_HEADER_NAME_ACCEPT_LANGUAGE (CharSlice99_from_str("Accept-Language"))

/**
 * `Allow`.
 */
#define SMOLRTSP_HEADER_NAME_ALLOW (CharSlice99_from_str("Allow"))

/**
 * `Authorization`.
 */
#define SMOLRTSP_HEADER_NAME_AUTHORIZATION (CharSlice99_from_str("Authorization"))

/**
 * `Bandwidth`.
 */
#define SMOLRTSP_HEADER_NAME_BANDWIDTH (CharSlice99_from_str("Bandwidth"))

/**
 * `Blocksize`.
 */
#define SMOLRTSP_HEADER_NAME_BLOCKSIZE (CharSlice99_from_str("Blocksize"))

/**
 * `Cache-Control`.
 */
#define SMOLRTSP_HEADER_NAME_CACHE_CONTROL (CharSlice99_from_str("Cache-Control"))

/**
 * `Conference`.
 */
#define SMOLRTSP_HEADER_NAME_CONFERENCE (CharSlice99_from_str("Conference"))

/**
 * `Connection`.
 */
#define SMOLRTSP_HEADER_NAME_CONNECTION (CharSlice99_from_str("Connection"))

/**
 * `Content-Base`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_BASE (CharSlice99_from_str("Content-Base"))

/**
 * `Content-Encoding`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_ENCODING (CharSlice99_from_str("Content-Encoding"))

/**
 * `Content-Language`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_LANGUAGE (CharSlice99_from_str("Content-Language"))

/**
 * `Content-Length`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_LENGTH (CharSlice99_from_str("Content-Length"))

/**
 * `Content-Location"`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_LOCATION (CharSlice99_from_str("Content-Location"))

/**
 * `Content-Type`.
 */
#define SMOLRTSP_HEADER_NAME_CONTENT_TYPE (CharSlice99_from_str("Content-Type"))

/**
 * `CSeq`.
 */
#define SMOLRTSP_HEADER_NAME_C_SEQ (CharSlice99_from_str("CSeq"))

/**
 * `Date`.
 */
#define SMOLRTSP_HEADER_NAME_DATE (CharSlice99_from_str("Date"))

/**
 * `Expires`.
 */
#define SMOLRTSP_HEADER_NAME_EXPIRES (CharSlice99_from_str("Expires"))

/**
 * `From`.
 */
#define SMOLRTSP_HEADER_NAME_FROM (CharSlice99_from_str("From"))

/**
 * `If-Modified-Since`.
 */
#define SMOLRTSP_HEADER_NAME_IF_MODIFIED_SINCE (CharSlice99_from_str("If-Modified-Since"))

/**
 * `"Last-Modified`.
 */
#define SMOLRTSP_HEADER_NAME_LAST_MODIFIED (CharSlice99_from_str("Last-Modified"))

/**
 * `Proxy-Authenticate`.
 */
#define SMOLRTSP_HEADER_NAME_PROXY_AUTHENTICATE (CharSlice99_from_str("Proxy-Authenticate"))

/**
 * `Proxy-Require`.
 */
#define SMOLRTSP_HEADER_NAME_PROXY_REQUIRE (CharSlice99_from_str("Proxy-Require"))

/**
 * `Public`.
 */
#define SMOLRTSP_HEADER_NAME_PUBLIC (CharSlice99_from_str("Public"))

/**
 * `Range`.
 */
#define SMOLRTSP_HEADER_NAME_RANGE (CharSlice99_from_str("Range"))

/**
 * `Referrer`.
 */
#define SMOLRTSP_HEADER_NAME_REFERER (CharSlice99_from_str("Referrer"))

/**
 * `Require`.
 */
#define SMOLRTSP_HEADER_NAME_REQUIRE (CharSlice99_from_str("Require"))

/**
 * `Retry-After`.
 */
#define SMOLRTSP_HEADER_NAME_RETRY_AFTER (CharSlice99_from_str("Retry-After"))

/**
 * `RTP-Info`.
 */
#define SMOLRTSP_HEADER_NAME_RTP_INFO (CharSlice99_from_str("RTP-Info"))

/**
 * `Scale`.
 */
#define SMOLRTSP_HEADER_NAME_SCALE (CharSlice99_from_str("Scale"))

/**
 * `Session`.
 */
#define SMOLRTSP_HEADER_NAME_SESSION (CharSlice99_from_str("Session"))

/**
 * `Server`.
 */
#define SMOLRTSP_HEADER_NAME_SERVER (CharSlice99_from_str("Server"))

/**
 * `Speed`.
 */
#define SMOLRTSP_HEADER_NAME_SPEED (CharSlice99_from_str("Speed"))

/**
 * `Transport`.
 */
#define SMOLRTSP_HEADER_NAME_TRANSPORT (CharSlice99_from_str("Transport"))

/**
 * `Unsupported`.
 */
#define SMOLRTSP_HEADER_NAME_UNSUPPORTED (CharSlice99_from_str("Unsupported"))

/**
 * `User-Agent`.
 */
#define SMOLRTSP_HEADER_NAME_USER_AGENT (CharSlice99_from_str("User-Agent"))

/**
 * `Via`.
 */
#define SMOLRTSP_HEADER_NAME_VIA (CharSlice99_from_str("Via"))

/**
 * `WWW-Authenticate`.
 */
#define SMOLRTSP_HEADER_NAME_WWW_AUTHENTICATE (CharSlice99_from_str("WWW-Authenticate"))

#endif // SMOLRTSP_HEADER_H
