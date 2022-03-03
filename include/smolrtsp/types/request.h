/**
 * @file
 * @brief An RTSP request.
 */

#include <smolrtsp/types/error.h>
#include <smolrtsp/types/header_map.h>
#include <smolrtsp/types/message_body.h>
#include <smolrtsp/types/request_line.h>

#include <stdbool.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * An RTSP request.
 */
typedef struct {
    /**
     * The request line.
     */
    SmolRTSP_RequestLine start_line;

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
} SmolRTSP_Request;

/**
 * Returns an RTSP request suitable for being parsed.
 */
SmolRTSP_Request SmolRTSP_Request_uninit(void);

/**
 * Serialises @p self into @p w.
 *
 * If `CSeq` and `Content-Length` are not present in
 * #SmolRTSP_Request.header_map, they will be taken from #SmolRTSP_Request.cseq
 * and #SmolRTSP_Request.body, respectively, and serialised as first headers
 * automatically.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The writer to be provided with serialised data.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_Request_serialize(SmolRTSP_Request self, SmolRTSP_Writer w)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_Request_parse(
    SmolRTSP_Request *restrict self, CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 */
bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs)
    SMOLRTSP_PRIV_MUST_USE;
