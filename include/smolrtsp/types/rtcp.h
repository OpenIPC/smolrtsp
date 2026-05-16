/**
 * @file
 * @brief <a
 * href="https://datatracker.ietf.org/doc/html/rfc3550#section-6.4">RFC 3550
 * §6.4</a>-compliant RTCP Sender Report (SR) implementation.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * RTCP packet type for Sender Report (RFC 3550 §6.4.1).
 */
#define SMOLRTSP_RTCP_PT_SR 200

/**
 * The fixed wire size of an RTCP Sender Report with no reception report
 * blocks (RC = 0).
 *
 * 4 (common header) + 4 (sender SSRC) + 8 (NTP timestamp) + 4 (RTP timestamp)
 * + 4 (packet count) + 4 (octet count) = 28 bytes.
 */
#define SMOLRTSP_RTCP_SR_SIZE_BASE 28u

/**
 * An RTCP Sender Report (SR) header.
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |V=2|P|    RC   |   PT=SR=200   |             length            |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                         SSRC of sender                        |
 * +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * |              NTP timestamp, most significant word             |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |             NTP timestamp, least significant word             |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                         RTP timestamp                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                     sender's packet count                     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                      sender's octet count                     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * All multi-byte numerical fields must be in network byte order, with the
 * exception of #ssrc which is treated as opaque (sender and receiver compare
 * it as a bit pattern, so either endianness is acceptable provided it stays
 * consistent with the matching RTP stream).
 *
 * Reception report blocks (RC > 0) and chained SDES/BYE packets are not yet
 * supported by this serializer; #rc must be zero.
 */
typedef struct {
    /**
     * (1 bit) Padding flag. When set, the SR ends with extra padding octets
     * whose count is given in the last byte. Typically `false`.
     */
    bool padding;

    /**
     * (5 bits) Reception report count. Must be `0` for this implementation.
     */
    uint8_t rc;

    /**
     * (32 bits) Synchronization source identifier of the sender. Must match
     * the SSRC of the corresponding RTP stream. Treated as opaque on the
     * wire.
     */
    uint32_t ssrc;

    /**
     * (32 bits) NTP-epoch seconds. NTP epoch is 1900-01-01 UTC; converting
     * a Unix-epoch value requires adding 2208988800. Network byte order.
     */
    uint32_t ntp_sec;

    /**
     * (32 bits) NTP fractional seconds. 2^32 represents one full second.
     * Network byte order.
     */
    uint32_t ntp_frac;

    /**
     * (32 bits) The RTP timestamp that corresponds to the same instant as
     * the NTP timestamp above. Receivers use this pair to map any RTP
     * timestamp to wall-clock time. Network byte order.
     */
    uint32_t rtp_ts;

    /**
     * (32 bits) Total number of RTP data packets transmitted by the sender
     * since starting transmission, up to the time this SR was generated.
     * Network byte order.
     */
    uint32_t pkt_count;

    /**
     * (32 bits) Total number of payload octets (i.e., not including header
     * or padding) transmitted by the sender since starting transmission.
     * Network byte order.
     */
    uint32_t octet_count;
} SmolRTSP_RtcpSr;

/**
 * Computes the size of the binary @p self.
 *
 * Currently always returns #SMOLRTSP_RTCP_SR_SIZE_BASE; reserved for future
 * support of reception report blocks.
 */
size_t SmolRTSP_RtcpSr_size(SmolRTSP_RtcpSr self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Writes @p self to @p buffer.
 *
 * @param[in] self The SR packet to write.
 * @param[out] buffer The pointer to write to. Must be at least of size
 * `SmolRTSP_RtcpSr_size(self)`.
 *
 * @return The pointer to the passed buffer.
 *
 * @pre `buffer != NULL`
 * @pre `self.rc == 0` (reception report blocks not yet supported)
 */
uint8_t *SmolRTSP_RtcpSr_serialize(
    SmolRTSP_RtcpSr self, uint8_t buffer[restrict]) SMOLRTSP_PRIV_MUST_USE;
