/**
 * @file
 * @brief <a href="https://datatracker.ietf.org/doc/html/rfc3550">RFC
 * 3550</a>-compliant RTP implementation.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * An RTP header.
 *
 * All numerical fields must be in network byte order.
 */
typedef struct {
    /**
     * (2 bits) Indicates the version of the protocol. Current version is 2.
     */
    uint8_t version;

    /**
     * (1 bit) Used to indicate if there are extra padding bytes at the end of
     * the RTP packet. Padding may be used to fill up a block of certain size,
     * for example as required by an encryption algorithm. The last byte of the
     * padding contains the number of padding bytes that were added (including
     * itself).
     */
    bool padding;

    /**
     * (1 bit) Indicates presence of an extension header between the header and
     * payload data. The extension header is application or profile specific.
     */
    bool extension;

    /**
     * (4 bits) Contains the number of CSRC identifiers (defined below) that
     * follow the SSRC (also defined below).
     */
    uint8_t csrc_count;

    /**
     * (1 bit) Signaling used at the application level in a profile-specific
     * manner. If it is set, it means that the current data has some special
     * relevance for the application.
     */
    bool marker;

    /**
     * (7 bits) Indicates the format of the payload and thus determines its
     * interpretation by the application. Values are profile specific and may be
     * dynamically assigned.
     */
    uint8_t payload_ty;

    /**
     * (16 bits) The sequence number is incremented for each RTP data packet
     * sent and is to be used by the receiver to detect packet loss[1] and to
     * accommodate out-of-order delivery. The initial value of the sequence
     * number should be randomized to make known-plaintext attacks on Secure
     * Real-time Transport Protocol more difficult.
     */
    uint16_t sequence_number;

    /**
     * Used by the receiver to play back the received samples at appropriate
     * time and interval. When several media streams are present, the timestamps
     * may be independent in each stream.[b] The granularity of the timing is
     * application specific. For example, an audio application that samples data
     * once every 125 μs (8 kHz, a common sample rate in digital telephony)
     * would use that value as its clock resolution. Video streams typically use
     * a 90 kHz clock. The clock granularity is one of the details that is
     * specified in the RTP profile for an application.
     */
    uint32_t timestamp;

    /**
     * (32 bits) Synchronization source identifier uniquely identifies the
     * source of a stream. The synchronization sources within the same RTP
     * session will be unique.
     */
    uint32_t ssrc;

    /**
     * (32 bits each, the number of entries is indicated by the CSRC count
     * field) Contributing source IDs enumerate contributing sources to a stream
     * which has been generated from multiple sources.
     */
    uint32_t *csrc;

    /**
     * (optional, presence indicated by Extension field) The first 32-bit word
     * contains a profile-specific identifier (16 bits) and a length specifier
     * (16 bits) that indicates the length of the extension in 32-bit units,
     * excluding the 32 bits of the extension header. The extension header data
     * follows.
     */
    uint16_t extension_profile;

    /**
     * Indicates the length of the extension in 32-bit units.
     */
    uint16_t extension_payload_len;

    /**
     * The pointer to extension header data.
     */
    uint8_t *extension_payload;
} SmolRTSP_RtpHeader;

/**
 * Computes the size of the binary @p self.
 */
size_t SmolRTSP_RtpHeader_size(SmolRTSP_RtpHeader self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Writes @p self to @p buffer.
 *
 * @param[in] self The RTP header to write.
 * @param[out] buffer The pointer to write to. Must be at least of size
 * `SmolRTSP_RtpHeader_size(self)`.
 *
 * @return The pointer to a passed buffer.
 */
uint8_t *SmolRTSP_RtpHeader_serialize(
    SmolRTSP_RtpHeader self, uint8_t buffer[restrict]) SMOLRTSP_PRIV_MUST_USE;
