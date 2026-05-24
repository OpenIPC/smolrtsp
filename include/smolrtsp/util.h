/**
 * @file
 * @brief Utilitary stuff.
 */

#pragma once

#include <smolrtsp/option.h>
#include <smolrtsp/types/error.h>

#include <stdint.h>

#include <slice99.h>

/**
 * Carriage-return + new-line represented as a data slice.
 */
#define SMOLRTSP_CRLF (CharSlice99_from_str("\r\n"))

/**
 * The default RTSP port.
 */
#define SMOLRTSP_DEFAULT_PORT 554

/**
 * An RTSP lower transport.
 */
typedef enum {
    /**
     * TCP.
     */
    SmolRTSP_LowerTransport_TCP,

    /**
     * UDP.
     */
    SmolRTSP_LowerTransport_UDP,
} SmolRTSP_LowerTransport;

/**
 * Converts @p self to a string representation (`"TCP"` for
 * #SmolRTSP_LowerTransport_TCP and `"UDP"` for #SmolRTSP_LowerTransport_UDP).
 */
const char *SmolRTSP_LowerTransport_str(SmolRTSP_LowerTransport self);

/**
 * An RTP/RTCP port pair specified as a range, e.g., `client_port=3456-3457`.
 */
typedef struct {
    /**
     * The RTP port.
     */
    uint16_t rtp_port;

    /**
     * The RTCP port.
     */
    uint16_t rtcp_port;
} SmolRTSP_PortPair;

/**
 * Defines `SmolRTSP_PortPairOption`.
 *
 * See [Datatype99](https://github.com/hirrolot/datatype99) for the macro usage.
 */
SMOLRTSP_DEF_OPTION(SmolRTSP_PortPair);

/**
 * An RTP/RTCP channel pair specified as a range, e.g., `interleaved=4-5`.
 */
typedef struct {
    /**
     * The RTP channel identifier.
     */
    uint8_t rtp_channel;

    /**
     * The RTCP channel identifier.
     */
    uint8_t rtcp_channel;
} SmolRTSP_ChannelPair;

/**
 * Defines `SmolRTSP_ChannelPairOption`.
 *
 * See [Datatype99](https://github.com/hirrolot/datatype99) for the macro usage.
 */
SMOLRTSP_DEF_OPTION(SmolRTSP_ChannelPair);

/**
 * The RTSP transport configuration.
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc2326#section-12.39>
 */
typedef struct {
    /**
     * The lower level transport (TCP or UDP).
     */
    SmolRTSP_LowerTransport lower;

    /**
     * True if the `unicast` parameter is present.
     */
    bool unicast;

    /**
     * True if the `multicast` parameter is present.
     */
    bool multicast;

    /**
     * The `interleaved` parameter, if present.
     */
    SmolRTSP_ChannelPairOption interleaved;

    /**
     * The `client_port` parameter, if present.
     */
    SmolRTSP_PortPairOption client_port;
} SmolRTSP_TransportConfig;

/**
 * Parses the
 * [`Transport`](https://datatracker.ietf.org/doc/html/rfc2326#section-12.39)
 * header.
 *
 * @param[out] config The result of parsing. It remains unchanged on failure.
 * @param[in] header_value The value of the `Transport` header.
 *
 * @return 0 on success, -1 on failure.
 *
 * @pre `config != NULL`
 */
int smolrtsp_parse_transport(
    SmolRTSP_TransportConfig *restrict config,
    CharSlice99 header_value) SMOLRTSP_PRIV_MUST_USE;

/**
 * Returns a four-octet interleaved binary data header.
 *
 * @param[in] channel_id The one-byte channel identifier.
 * @param[in] payload_len The length of the encapsulated binary data
 * (network byte order).
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc2326#section-10.12>
 */
uint32_t smolrtsp_interleaved_header(uint8_t channel_id, uint16_t payload_len)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Parses an four-octet interleaved binary data header @p data.
 *
 * @param[in] data The header to parse.
 * @param[out] channel_id The one-byte channel identifier.
 * @param[out] payload_len The length of the encapsulated binary data
 * (host byte order).
 *
 * @pre `channel_id != NULL`
 * @pre `payload_len != NULL`
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc2326#section-10.12>
 */
void smolrtsp_parse_interleaved_header(
    const uint8_t data[restrict static 4], uint8_t *restrict channel_id,
    uint16_t *restrict payload_len);

/**
 * The result of #smolrtsp_parse_interleaved_frame.
 */
typedef enum {
    /**
     * A complete interleaved frame was parsed.
     *
     * `channel_id`, `payload`, and `consumed` outputs are set.
     */
    SmolRTSP_InterleavedFrameStatus_Complete,

    /**
     * The input starts with the `$` magic byte but the full frame has not
     * yet arrived.
     *
     * Outputs are untouched. The caller should preserve the buffer and wait
     * for more bytes before retrying.
     */
    SmolRTSP_InterleavedFrameStatus_Partial,

    /**
     * The input does not begin with `$` and is therefore not an interleaved
     * binary frame.
     *
     * Outputs are untouched. The caller should fall through to whichever
     * other parser handles the protocol (typically
     * #SmolRTSP_Request_parse or #SmolRTSP_Response_parse).
     */
    SmolRTSP_InterleavedFrameStatus_NotInterleaved,
} SmolRTSP_InterleavedFrameStatus;

/**
 * Parses one interleaved binary frame
 * ([RFC 2326
 * §10.12](https://datatracker.ietf.org/doc/html/rfc2326#section-10.12)).
 *
 * Used by RTSP servers that want to receive client-pushed media (RTSP back
 * channel / ONVIF Profile T) on a TCP-interleaved RTSP connection. Frames
 * look like `$<channel:u8><length:u16-be><payload>` on the wire.
 *
 * @param[in] input The raw bytes at the head of the receive buffer.
 * @param[out] channel_id The one-byte channel identifier of the frame, set
 *             only on #SmolRTSP_InterleavedFrameStatus_Complete.
 * @param[out] payload A slice into @p input covering just the frame payload
 *             (no `$`/channel/length prefix), set only on Complete. The
 *             slice is valid as long as @p input remains valid.
 * @param[out] consumed The number of bytes (4-byte header + payload) the
 *             caller should drain from the buffer, set only on Complete.
 *
 * @return Frame status (see enum).
 *
 * @pre `channel_id != NULL`
 * @pre `payload != NULL`
 * @pre `consumed != NULL`
 */
SmolRTSP_InterleavedFrameStatus smolrtsp_parse_interleaved_frame(
    CharSlice99 input, uint8_t *restrict channel_id,
    U8Slice99 *restrict payload,
    size_t *restrict consumed) SMOLRTSP_PRIV_MUST_USE;
