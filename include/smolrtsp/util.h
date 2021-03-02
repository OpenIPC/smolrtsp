/**
 * @file
 * Utilitary stuff.
 */

#ifndef SMOLRTSP_UTIL_H
#define SMOLRTSP_UTIL_H

#include <smolrtsp/common.h>

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
 * Extracts the lower transport from the header value @p value.
 *
 * @param[in] value The value of the `Transport` header.
 *
 * @return -1 on error, 0 on success.
 *
 * @pre `result != NULL`
 */
int SmolRTSP_parse_lower_transport(SmolRTSP_LowerTransport *restrict result, Slice99 value);

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
int SmolRTSP_parse_client_port(int *restrict rtp_port, int *restrict rtcp_port, Slice99 value);

#endif // SMOLRTSP_UTIL_H
