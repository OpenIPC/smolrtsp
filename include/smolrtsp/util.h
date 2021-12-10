/**
 * @file
 * Utilitary stuff.
 */

#ifndef SMOLRTSP_UTIL_H
#define SMOLRTSP_UTIL_H

#include <smolrtsp/error.h>

#include <stdint.h>

#include <slice99.h>

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
 * Converts @p self to a string representation (`"TCP"` for #SmolRTSP_LowerTransport_TCP and `"UDP"`
 * for #SmolRTSP_LowerTransport_UDP).
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
int SmolRTSP_parse_lower_transport(SmolRTSP_LowerTransport *restrict result, CharSlice99 value);

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
int SmolRTSP_parse_client_port(int *restrict rtp_port, int *restrict rtcp_port, CharSlice99 value);

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
 * @note If an RTCP channel ID is not specified, @p rtcp_chn_id will be set to -1.
 */
int SmolRTSP_parse_interleaved_chn_id(
    int *restrict rtp_chn_id, int *restrict rtcp_chn_id, CharSlice99 value);

/**
 * An interleaved binary data header used to mix RTSP requests/responses with streaming data in a
 * single TCP connection.
 */
typedef struct {
    /**
     * The one-byte channel identifier.
     */
    uint8_t channel_id;

    /**
     * The length of the encapsulated binary data (network byte order).
     */
    uint16_t payload_len;
} SmolRTSP_InterleavedDataHeader;

/**
 * Returns #SmolRTSP_InterleavedDataHeader represented as `uint32_t`.
 */
uint32_t SmolRTSP_InterleavedDataHeader_as_u32(SmolRTSP_InterleavedDataHeader self);

/**
 * Returns #SmolRTSP_InterleavedDataHeader from `uint32_t`.
 */
SmolRTSP_InterleavedDataHeader SmolRTSP_InterleavedDataHeader_from_u32(uint32_t data);

#endif // SMOLRTSP_UTIL_H
