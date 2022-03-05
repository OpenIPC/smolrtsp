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
#define SMOLRTSP_CRLF (CharSlice99_from_str("\r\n"))

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
     * The RTP port part.
     */
    uint16_t rtp_port;

    /**
     * The RTCP port part.
     */
    uint16_t rtcp_port;
} SmolRTSP_PortPair;

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
 * Parses a header value parameter in the form
 * `<param-name>=%<num-specifier>-%<num-specifier>`.
 *
 * @param[out] num_0 The address of the first number.
 * @param[out] num_1 The address of the second number.
 * @param[in] num_specifier The `scanf`-like specifier for the aforementioned
 * numbers.
 * @param[in] param_name The name of the parameter as in
 * #smolrtsp_parse_header_param.
 * @param[in] header_value The header value to search for the string.
 *
 * @pre `num_0 != NULL`
 * @pre `num_1 != NULL`
 * @pre @p num_specifier and @p param_name are null-terminated strings.
 */
int smolrtsp_parse_range(
    void *restrict num_0, void *restrict num_1,
    const char *restrict num_specifier, const char *restrict param_name,
    CharSlice99 header_value);

/**
 * Like #smolrtsp_parse_range but parses a port pair.
 *
 * @param[out] pair The destination address of the port pair.
 * @param[in] param_name The name of the parameter as in
 * #smolrtsp_parse_header_param.
 * @param[in] header_value The header value to search for the string.
 *
 * @pre `pair != NULL`
 * @pre @p param_name is a null-terminated string.
 */
int smolrtsp_parse_port_pair(
    SmolRTSP_PortPair *restrict pair, const char *restrict param_name,
    CharSlice99 header_value);

/**
 * Parses a header value parameter in the form `<param-name>=<param-value>`.
 *
 * @param[out] param_value The pointer to a parameter value, if found.
 * @param[in] param_name The name of a parameter to parse plus the `=` sign. For
 * example, `"mode="` or `"client_port="`.
 * @param[in] header_value The header value.
 *
 * @return 0 if @p param_name has been found, -1 otherwise.
 *
 * @pre @p param_name is a null-terminated string.
 * @pre `param_value != NULL`
 */
int smolrtsp_parse_header_param(
    CharSlice99 *restrict param_value, const char *restrict param_name,
    CharSlice99 header_value);

/**
 * Returns a four-octet interleaved binary data header.
 *
 * @param[in] channel_id The one-byte channel identifier.
 * @param[in] payload_len The length of the encapsulated binary data
 * (network byte order).
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc2326#section-10.12>
 */
uint32_t smolrtsp_interleaved_header(uint8_t channel_id, uint16_t payload_len);

/**
 * Parses an four-octet interleaved binary data header @p data.
 *
 * @param[in] data The header to parse.
 * @param[out] channel_id The one-byte channel identifier.
 * @param[out] payload_len The length of the encapsulated binary data
 * (network byte order).
 *
 * @pre `channel_id != NULL`
 * @pre `payload_len != NULL`
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc2326#section-10.12>
 */
void smolrtsp_parse_interleaved_header(
    uint32_t data, uint8_t *restrict channel_id,
    uint16_t *restrict payload_len);
