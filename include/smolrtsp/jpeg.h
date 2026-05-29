/**
 * @file
 * @brief JPEG / Motion JPEG RTP payload-header types and serialization.
 *
 * @see RTP Payload Format for JPEG-compressed Video:
 * <https://datatracker.ietf.org/doc/html/rfc2435>
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The size in bytes of an RFC 2435 main JPEG header (§3.1).
 *
 * Carries Type-Specific, Fragment Offset, Type, Q, Width and Height fields.
 * Present on every JPEG-over-RTP packet of a frame.
 */
#define SMOLRTSP_JPEG_MAIN_HEADER_SIZE 8

/**
 * The size in bytes of an RFC 2435 Quantization Table header (§3.1.8).
 *
 * Carries MBZ, Precision and Length fields, prefixing the QT data block
 * itself. Only emitted on the first RTP packet of a frame when an embedded
 * quantization table is sent (Q >= 128).
 */
#define SMOLRTSP_JPEG_QT_HEADER_SIZE 4

/**
 * The size in bytes of one 8-bit-precision baseline quantization table.
 */
#define SMOLRTSP_JPEG_QT_SIZE 64

/**
 * Default value for #SmolRTSP_JpegTransportConfig.max_packet_size: a
 * conservative MTU that leaves room for RTP / UDP / IP headers and one
 * 140-byte first-packet JPEG header (main + QT header + two 64-byte tables).
 */
#define SMOLRTSP_MAX_JPEG_PACKET_SIZE 1200

/**
 * The smallest Q-value that signals an embedded quantization table per
 * RFC 2435 §4.1. Q in `[0, 127]` references a pre-shared table; Q in
 * `[128, 255]` is paired with a #SmolRTSP_JpegQtHeader and the table bytes
 * themselves.
 */
#define SMOLRTSP_JPEG_Q_DYNAMIC 128

/**
 * The Type-specific field is unused in the baseline RFC 2435 profile.
 */
#define SMOLRTSP_JPEG_TYPE_SPECIFIC_DEFAULT 0

/**
 * An RFC 2435 main JPEG header (§3.1).
 *
 * Bit layout, transmitted in network byte order:
 *
 *      0                   1                   2                   3
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     | Type-specific |              Fragment Offset                  |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |      Type     |       Q       |     Width     |     Height    |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct {
    /** Type-specific field (byte 0). Set 0 for baseline RFC 2435 use. */
    uint8_t type_specific;
    /**
     * Fragment offset within the JPEG frame, in bytes (bytes 1..3, 24-bit
     * big-endian). The transport stamps this per packet; callers populate
     * 0 in a fresh header.
     */
    uint32_t fragment_offset;
    /** RTP/JPEG Type field (byte 4): subsampling + interlace flags. */
    uint8_t type;
    /**
     * Quality factor (byte 5). Values < #SMOLRTSP_JPEG_Q_DYNAMIC reference
     * the standard tables; values >= 128 mean a dynamic table is embedded.
     */
    uint8_t q;
    /** Picture width in 8-pixel units (byte 6). */
    uint8_t width_blocks;
    /** Picture height in 8-pixel units (byte 7). */
    uint8_t height_blocks;
} SmolRTSP_JpegPayloadHeader;

/**
 * Serializes @p self into @p buffer in network byte order.
 *
 * @param[in] self The header to write.
 * @param[out] buffer The memory area capable of storing
 * #SMOLRTSP_JPEG_MAIN_HEADER_SIZE bytes.
 */
void SmolRTSP_JpegPayloadHeader_serialize(
    SmolRTSP_JpegPayloadHeader self, uint8_t buffer[restrict]);

/**
 * An RFC 2435 Quantization Table header (§3.1.8).
 *
 * Bit layout, transmitted in network byte order:
 *
 *      0                   1                   2                   3
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |      MBZ      |   Precision   |             Length            |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct {
    /** MBZ (must be zero) field, byte 0. */
    uint8_t mbz;
    /**
     * Per-table precision bitmap, byte 1. Bit N set = table N uses 16-bit
     * coefficients (twice the size). 0 means all tables are 8-bit, which
     * is the baseline RFC 2435 use.
     */
    uint8_t precision;
    /**
     * Total length in bytes of the quantization-table data that follows
     * this header (bytes 2..3, big-endian). Two 8-bit tables = 128.
     */
    uint16_t length;
} SmolRTSP_JpegQtHeader;

/**
 * Serializes @p self into @p buffer in network byte order.
 *
 * @param[in] self The header to write.
 * @param[out] buffer The memory area capable of storing
 * #SMOLRTSP_JPEG_QT_HEADER_SIZE bytes.
 */
void SmolRTSP_JpegQtHeader_serialize(
    SmolRTSP_JpegQtHeader self, uint8_t buffer[restrict]);
