/**
 * @file
 * @brief An RTSP header.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/error.h>
#include <smolrtsp/writer.h>

#include <stdbool.h>
#include <stdio.h>

#include <unistd.h>

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
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `self != NULL`
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_Header_serialize(
    const SmolRTSP_Header *restrict self,
    SmolRTSP_Writer w) SMOLRTSP_PRIV_MUST_USE;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_Header_parse(
    SmolRTSP_Header *restrict self, CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_Header_eq(
    const SmolRTSP_Header *restrict lhs,
    const SmolRTSP_Header *restrict rhs) SMOLRTSP_PRIV_MUST_USE;

/**
 * `Accept`.
 */
#define SMOLRTSP_HEADER_ACCEPT (CharSlice99_from_str("Accept"))

/**
 * `Accept-Encoding`.
 */
#define SMOLRTSP_HEADER_ACCEPT_ENCODING                                        \
    (CharSlice99_from_str("Accept-Encoding"))

/**
 * `Accept-Language`.
 */
#define SMOLRTSP_HEADER_ACCEPT_LANGUAGE                                        \
    (CharSlice99_from_str("Accept-Language"))

/**
 * `Allow`.
 */
#define SMOLRTSP_HEADER_ALLOW (CharSlice99_from_str("Allow"))

/**
 * `Authorization`.
 */
#define SMOLRTSP_HEADER_AUTHORIZATION (CharSlice99_from_str("Authorization"))

/**
 * `Bandwidth`.
 */
#define SMOLRTSP_HEADER_BANDWIDTH (CharSlice99_from_str("Bandwidth"))

/**
 * `Blocksize`.
 */
#define SMOLRTSP_HEADER_BLOCKSIZE (CharSlice99_from_str("Blocksize"))

/**
 * `Cache-Control`.
 */
#define SMOLRTSP_HEADER_CACHE_CONTROL (CharSlice99_from_str("Cache-Control"))

/**
 * `Conference`.
 */
#define SMOLRTSP_HEADER_CONFERENCE (CharSlice99_from_str("Conference"))

/**
 * `Connection`.
 */
#define SMOLRTSP_HEADER_CONNECTION (CharSlice99_from_str("Connection"))

/**
 * `Content-Base`.
 */
#define SMOLRTSP_HEADER_CONTENT_BASE (CharSlice99_from_str("Content-Base"))

/**
 * `Content-Encoding`.
 */
#define SMOLRTSP_HEADER_CONTENT_ENCODING                                       \
    (CharSlice99_from_str("Content-Encoding"))

/**
 * `Content-Language`.
 */
#define SMOLRTSP_HEADER_CONTENT_LANGUAGE                                       \
    (CharSlice99_from_str("Content-Language"))

/**
 * `Content-Length`.
 */
#define SMOLRTSP_HEADER_CONTENT_LENGTH (CharSlice99_from_str("Content-Length"))

/**
 * `Content-Location"`.
 */
#define SMOLRTSP_HEADER_CONTENT_LOCATION                                       \
    (CharSlice99_from_str("Content-Location"))

/**
 * `Content-Type`.
 */
#define SMOLRTSP_HEADER_CONTENT_TYPE (CharSlice99_from_str("Content-Type"))

/**
 * `CSeq`.
 */
#define SMOLRTSP_HEADER_C_SEQ (CharSlice99_from_str("CSeq"))

/**
 * `Date`.
 */
#define SMOLRTSP_HEADER_DATE (CharSlice99_from_str("Date"))

/**
 * `Expires`.
 */
#define SMOLRTSP_HEADER_EXPIRES (CharSlice99_from_str("Expires"))

/**
 * `From`.
 */
#define SMOLRTSP_HEADER_FROM (CharSlice99_from_str("From"))

/**
 * `If-Modified-Since`.
 */
#define SMOLRTSP_HEADER_IF_MODIFIED_SINCE                                      \
    (CharSlice99_from_str("If-Modified-Since"))

/**
 * `"Last-Modified`.
 */
#define SMOLRTSP_HEADER_LAST_MODIFIED (CharSlice99_from_str("Last-Modified"))

/**
 * `Proxy-Authenticate`.
 */
#define SMOLRTSP_HEADER_PROXY_AUTHENTICATE                                     \
    (CharSlice99_from_str("Proxy-Authenticate"))

/**
 * `Proxy-Require`.
 */
#define SMOLRTSP_HEADER_PROXY_REQUIRE (CharSlice99_from_str("Proxy-Require"))

/**
 * `Public`.
 */
#define SMOLRTSP_HEADER_PUBLIC (CharSlice99_from_str("Public"))

/**
 * `Range`.
 */
#define SMOLRTSP_HEADER_RANGE (CharSlice99_from_str("Range"))

/**
 * `Referrer`.
 */
#define SMOLRTSP_HEADER_REFERER (CharSlice99_from_str("Referrer"))

/**
 * `Require`.
 */
#define SMOLRTSP_HEADER_REQUIRE (CharSlice99_from_str("Require"))

/**
 * `Retry-After`.
 */
#define SMOLRTSP_HEADER_RETRY_AFTER (CharSlice99_from_str("Retry-After"))

/**
 * `RTP-Info`.
 */
#define SMOLRTSP_HEADER_RTP_INFO (CharSlice99_from_str("RTP-Info"))

/**
 * `Scale`.
 */
#define SMOLRTSP_HEADER_SCALE (CharSlice99_from_str("Scale"))

/**
 * `Session`.
 */
#define SMOLRTSP_HEADER_SESSION (CharSlice99_from_str("Session"))

/**
 * `Server`.
 */
#define SMOLRTSP_HEADER_SERVER (CharSlice99_from_str("Server"))

/**
 * `Speed`.
 */
#define SMOLRTSP_HEADER_SPEED (CharSlice99_from_str("Speed"))

/**
 * `Transport`.
 */
#define SMOLRTSP_HEADER_TRANSPORT (CharSlice99_from_str("Transport"))

/**
 * `Unsupported`.
 */
#define SMOLRTSP_HEADER_UNSUPPORTED (CharSlice99_from_str("Unsupported"))

/**
 * `User-Agent`.
 */
#define SMOLRTSP_HEADER_USER_AGENT (CharSlice99_from_str("User-Agent"))

/**
 * `Via`.
 */
#define SMOLRTSP_HEADER_VIA (CharSlice99_from_str("Via"))

/**
 * `WWW-Authenticate`.
 */
#define SMOLRTSP_HEADER_WWW_AUTHENTICATE                                       \
    (CharSlice99_from_str("WWW-Authenticate"))
