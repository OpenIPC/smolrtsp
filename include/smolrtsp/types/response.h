/**
 * @file
 * @brief An RTSP response.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/error.h>
#include <smolrtsp/types/header_map.h>
#include <smolrtsp/types/message_body.h>
#include <smolrtsp/types/response_line.h>

#include <stdbool.h>

#include <slice99.h>

/**
 * An RTSP response.
 */
typedef struct {
    /**
     * The response line.
     */
    SmolRTSP_ResponseLine start_line;

    /**
     * The header map.
     */
    SmolRTSP_HeaderMap header_map;

    /**
     * The message body.
     */
    SmolRTSP_MessageBody body;

    /**
     * The sequence number for an RTSP request/response pair.
     */
    uint32_t cseq;
} SmolRTSP_Response;

/**
 * Returns an RTSP response suitable for being parsed.
 */
SmolRTSP_Response SmolRTSP_Response_uninit(void) SMOLRTSP_PRIV_MUST_USE;

/**
 * Serialises @p self into @p w.
 *
 * If `CSeq` and `Content-Length` are not present in
 * #SmolRTSP_Response.header_map, they will be taken from
 * #SmolRTSP_Response.cseq and #SmolRTSP_Response.body, respectively, and
 * serialised as first headers automatically.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The writer to be provided with serialised data.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `self != NULL`
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_Response_serialize(
    const SmolRTSP_Response *restrict self,
    SmolRTSP_Writer w) SMOLRTSP_PRIV_MUST_USE;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_Response_parse(
    SmolRTSP_Response *restrict self, CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_Response_eq(
    const SmolRTSP_Response *restrict lhs,
    const SmolRTSP_Response *restrict rhs) SMOLRTSP_PRIV_MUST_USE;
