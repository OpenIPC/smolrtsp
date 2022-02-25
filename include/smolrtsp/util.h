/**
 * @file
 * @brief Utilitary stuff.
 */

#pragma once

#include <smolrtsp/types/error.h>

#include <stdint.h>

#include <slice99.h>

/**
 * Carriage-return + new-line represented as a data slice.
 */
#define SMOLRTSP_CRLF CharSlice99_from_str("\r\n")

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
 * Extracts the lower transport from the header value @p value.
 *
 * @param[in] value The value of the `Transport` header.
 *
 * @return -1 on error, 0 on success.
 *
 * @pre `result != NULL`
 */
int SmolRTSP_parse_lower_transport(
    SmolRTSP_LowerTransport *restrict result, CharSlice99 value);

/**
 * Extracts the `client_port` parameter from the header value @p value.
 *
 * @param[in] value The value of the `Transport` header.
 *
 * @return -1 on error, 0 on success.
 *
 * @pre `rtp_port != NULL`
 * @pre `rtcp_port != NULL`
 */
int SmolRTSP_parse_client_port(
    int *restrict rtp_port, int *restrict rtcp_port, CharSlice99 value);

/**
 * Extracts the `interleaved` parameter from the header value @p value.
 *
 * @param[in] value The value of the `Transport` header.
 *
 * @return -1 on error, 0 on success.
 *
 * @pre `rtp_chn_id != NULL`
 * @pre `rtcp_chn_id != NULL`
 *
 * @note If an RTCP channel ID is not specified, @p rtcp_chn_id will be set to
 * -1.
 */
int SmolRTSP_parse_interleaved_chn_id(
    int *restrict rtp_chn_id, int *restrict rtcp_chn_id, CharSlice99 value);

/**
 * Returns a four-octet interleaved binary data header.
 *
 * @param[in] channel_id The one-byte channel identifier.
 * @param[in] payload_len The length of the encapsulated binary data (network
 * byte order).
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc2326#section-10.12>
 */
uint32_t smolrtsp_interleaved_header(uint8_t channel_id, uint16_t payload_len);

/**
 * Parses an four-octet interleaved binary data header @p data.
 *
 * @param[out] channel_id The one-byte channel identifier.
 * @param[out] payload_len The length of the encapsulated binary data (network
 * byte order).
 *
 * @pre `channel_id != NULL`
 * @pre `payload_len != NULL`
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc2326#section-10.12>
 */
void smolrtsp_parse_interleaved_header(
    uint32_t data, uint8_t *restrict channel_id,
    uint16_t *restrict payload_len);
