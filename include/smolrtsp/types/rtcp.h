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
 * RTCP packet type for Receiver Report (RFC 3550 §6.4.2).
 */
#define SMOLRTSP_RTCP_PT_RR 201

/**
 * RTCP packet type for Source Description (RFC 3550 §6.5).
 */
#define SMOLRTSP_RTCP_PT_SDES 202

/**
 * RTCP packet type for Goodbye (RFC 3550 §6.6).
 */
#define SMOLRTSP_RTCP_PT_BYE 203

/**
 * SDES item type for canonical end-point identifier (CNAME)
 * (RFC 3550 §6.5.1).
 */
#define SMOLRTSP_RTCP_SDES_CNAME 1

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

/**
 * The fixed wire size of an RTCP Receiver Report with no reception
 * report blocks (RC = 0): a 4-byte common header plus the 4-byte SSRC
 * of the packet sender = 8 bytes.
 */
#define SMOLRTSP_RTCP_RR_SIZE_BASE 8u

/**
 * An RTCP Receiver Report (RR) header (RFC 3550 §6.4.2).
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |V=2|P|    RC   |   PT=RR=201   |             length            |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                     SSRC of packet sender                     |
 * +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * Reception report blocks (RC > 0) are not yet supported; #rc must be
 * zero.
 */
typedef struct {
    /**
     * (1 bit) Padding flag.
     */
    bool padding;

    /**
     * (5 bits) Reception report count. Must be `0` for this implementation.
     */
    uint8_t rc;

    /**
     * (32 bits) Synchronization source identifier of the packet sender.
     * Network byte order; SSRC is opaque on the wire so either endianness
     * works as long as it stays consistent with the matching RTP stream.
     */
    uint32_t ssrc;
} SmolRTSP_RtcpRr;

/**
 * Computes the size of the binary @p self. Currently always returns
 * #SMOLRTSP_RTCP_RR_SIZE_BASE.
 */
size_t SmolRTSP_RtcpRr_size(SmolRTSP_RtcpRr self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Writes @p self to @p buffer.
 *
 * @param[in] self The RR packet to write.
 * @param[out] buffer Must be at least `SmolRTSP_RtcpRr_size(self)` bytes.
 *
 * @return The pointer to the passed buffer.
 *
 * @pre `buffer != NULL`
 * @pre `self.rc == 0`
 */
uint8_t *SmolRTSP_RtcpRr_serialize(
    SmolRTSP_RtcpRr self, uint8_t buffer[restrict]) SMOLRTSP_PRIV_MUST_USE;

/**
 * An RTCP SDES packet with a single chunk containing a single CNAME
 * item (RFC 3550 §6.5). This is the minimum useful SDES form and the
 * one most senders emit alongside an SR/RR for receiver bookkeeping.
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |V=2|P|   SC=1   |  PT=SDES=202 |             length            |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                          SSRC                                 |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | CNAME=1 |  len  | user@host (UTF-8) ...                       |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | 0  (item terminator) | zero padding to 32-bit boundary        |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct {
    /**
     * (1 bit) Padding flag.
     */
    bool padding;

    /**
     * (32 bits) SSRC of the source being described. Opaque; same caveat
     * as #SmolRTSP_RtcpSr.ssrc.
     */
    uint32_t ssrc;

    /**
     * Canonical name (CNAME) in `user@host`-or-`host` form. Must not be
     * NULL and must be ≤ 255 bytes (the SDES item length field is 8 bits).
     */
    const char *cname;
} SmolRTSP_RtcpSdesCname;

/**
 * Computes the size of the binary @p self.
 *
 * Layout: 4-byte common header + 4-byte SSRC + 2-byte CNAME item header
 * + N bytes of CNAME text + 1-byte item-list terminator, then zero-padded
 * to a 32-bit boundary.
 *
 * @pre `self.cname != NULL`
 * @pre `strlen(self.cname) <= 255`
 */
size_t
SmolRTSP_RtcpSdesCname_size(SmolRTSP_RtcpSdesCname self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Writes @p self to @p buffer.
 *
 * @pre `buffer != NULL`
 * @pre `self.cname != NULL`
 * @pre `strlen(self.cname) <= 255`
 */
uint8_t *SmolRTSP_RtcpSdesCname_serialize(
    SmolRTSP_RtcpSdesCname self,
    uint8_t buffer[restrict]) SMOLRTSP_PRIV_MUST_USE;

/**
 * An RTCP BYE packet for a single SSRC, optionally carrying a reason
 * text (RFC 3550 §6.6).
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |V=2|P|   SC=1   |  PT=BYE=203  |             length            |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                           SSRC                                |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | (opt) len | reason text ...                                   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct {
    /**
     * (1 bit) Padding flag.
     */
    bool padding;

    /**
     * (32 bits) SSRC of the source leaving the session.
     */
    uint32_t ssrc;

    /**
     * Optional human-readable reason text (NULL or empty string means
     * no reason). Must be ≤ 255 bytes; the length field is 8 bits.
     */
    const char *reason;
} SmolRTSP_RtcpBye;

/**
 * Computes the size of the binary @p self.
 *
 * Without a reason: 8 bytes (4 header + 4 SSRC). With a reason: the
 * above plus 1 byte length + N bytes text, zero-padded to a 32-bit
 * boundary.
 *
 * @pre `self.reason == NULL || strlen(self.reason) <= 255`
 */
size_t SmolRTSP_RtcpBye_size(SmolRTSP_RtcpBye self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Writes @p self to @p buffer.
 *
 * @pre `buffer != NULL`
 * @pre `self.reason == NULL || strlen(self.reason) <= 255`
 */
uint8_t *SmolRTSP_RtcpBye_serialize(
    SmolRTSP_RtcpBye self, uint8_t buffer[restrict]) SMOLRTSP_PRIV_MUST_USE;
