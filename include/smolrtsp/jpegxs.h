/**
 * @file
 * @brief JPEG XS RTP payload-header types and serialization.
 *
 * @see RTP Payload Format for ISO/IEC 21122 (JPEG XS):
 * <https://datatracker.ietf.org/doc/html/rfc9134>
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The size in bytes of a JPEG XS RTP payload header (RFC 9134 §4.3).
 */
#define SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE 4

/**
 * JPEG XS packetization mode -- the K bit of the payload header
 * (RFC 9134 §4.3).
 */
typedef enum {
    /** K=0: codestream mode. The whole codestream of a frame (or field)
     *  forms one packetization unit; packet boundaries are MTU-driven. */
    SMOLRTSP_JPEGXS_PACK_CODESTREAM = 0,
    /** K=1: slice mode. The header segment and each slice are their own
     *  packetization units. */
    SMOLRTSP_JPEGXS_PACK_SLICE = 1,
} SmolRTSP_JpegXsPackMode;

/**
 * Interlaced-scan information -- the I field (2 bits) of the payload header
 * (RFC 9134 §4.3).
 */
typedef enum {
    /** I=00: progressively scanned. */
    SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE = 0,
    /** I=10: first field of an interlaced frame. */
    SMOLRTSP_JPEGXS_INTERLACE_FIELD_1 = 2,
    /** I=11: second field of an interlaced frame. */
    SMOLRTSP_JPEGXS_INTERLACE_FIELD_2 = 3,
} SmolRTSP_JpegXsInterlace;

/**
 * The SEP-counter sentinel that identifies the header-segment packetization
 * unit in slice mode (K=1). See RFC 9134 §4.3.
 */
#define SMOLRTSP_JPEGXS_SEP_HEADER_SEGMENT 0x7FF

/**
 * A JPEG XS RTP payload header (RFC 9134 §4.3).
 *
 * Bit layout, transmitted in network byte order:
 *
 *      0                   1                   2                   3
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |T|K|L| I |F counter|     SEP counter     |     P counter       |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct {
    /** T -- packets are sent sequentially by the transmitter. */
    bool sequential;
    /** K -- packetization mode. */
    SmolRTSP_JpegXsPackMode pack_mode;
    /** L -- last packet of the current packetization unit. */
    bool last_in_unit;
    /** I -- 2-bit interlaced-scan field. */
    SmolRTSP_JpegXsInterlace interlace;
    /** F -- frame counter, mod 32 (5 bits). */
    uint8_t frame_counter;
    /** SEP -- slice/extended-packet counter, 11 bits. */
    uint16_t sep_counter;
    /** P -- packet counter within the unit, mod 2048 (11 bits). */
    uint16_t packet_counter;
} SmolRTSP_JpegXsPayloadHeader;

/**
 * Serializes @p self into @p buffer in network byte order.
 *
 * @param[in] self The header to write.
 * @param[out] buffer The memory area capable of storing
 * #SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE bytes.
 */
void SmolRTSP_JpegXsPayloadHeader_serialize(
    SmolRTSP_JpegXsPayloadHeader self, uint8_t buffer[restrict]);
