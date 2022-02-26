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
 * Parses a lower transport from @p value (either TCP or UDP).
 *
 * @param[in] value The
 * [`Transport`](https://datatracker.ietf.org/doc/html/rfc2326#section-12.39)
 * header value.
 *
 * @return #SmolRTSP_LowerTransport or -1 on parsing error.
 */
int smolrtsp_parse_lower_transport(CharSlice99 value);

/**
 * Parses a header value parameter in the form `<param-name>=<param-value>`.
 *
 * @param[in] param_name The name of a parameter to parse plus the `=` sign. For
 * example, `"mode="` or `"client_port="`.
 * @param[in] value The header value.
 * @param[out] param_value The pointer to a parameter value, if found.
 *
 * @return Whether @p param_name has been found or not.
 *
 * @pre `param_name` is a null-terminated string.
 */
bool smolrtsp_parse_header_param(
    const char *restrict param_name, CharSlice99 value,
    CharSlice99 *restrict param_value);

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
