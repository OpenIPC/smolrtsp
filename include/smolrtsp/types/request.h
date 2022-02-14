/**
 * @file
 * An RTSP request.
 */

#include <smolrtsp/types/error.h>
#include <smolrtsp/types/header_map.h>
#include <smolrtsp/types/message_body.h>
#include <smolrtsp/types/request_line.h>

#include <stdbool.h>

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
} SmolRTSP_Request;

/**
 * Serialises @p self into @p w.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The writer to be provided with serialised data.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_Request_serialize(SmolRTSP_Request self, SmolRTSP_Writer w);

/**
 * A state of parsing of #SmolRTSP_Request.
 */
typedef enum {
    SmolRTSP_RequestParseState_RequestLine,
    SmolRTSP_RequestParseState_HeaderMap,
    SmolRTSP_RequestParseState_MessageBody,
    SmolRTSP_RequestParseState_Done,
} SmolRTSP_RequestParseState;

/**
 * Parses @p data to @p self.
 *
 * @pre `self != NULL`
 * @pre `state != NULL`
 */
SmolRTSP_ParseResult SmolRTSP_Request_parse(
    SmolRTSP_Request *restrict self, CharSlice99 input, SmolRTSP_RequestParseState *restrict state);

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @return `true` if @p lhs and @p rhs are equal, `false` otherwise.
 */
bool SmolRTSP_Request_eq(SmolRTSP_Request lhs, SmolRTSP_Request rhs);
