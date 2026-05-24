/**
 * @file
 * @brief AV1 RTP aggregation-header types, LEB128 helpers, and OBU constants.
 *
 * @see AOMedia AV1 RTP Specification:
 * <https://aomediacodec.github.io/av1-rtp-spec/>
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The size in bytes of the AV1 RTP aggregation header (AOMedia AV1 RTP
 * §4.4). Each RTP packet's payload begins with this single byte before
 * any OBU elements.
 */
#define SMOLRTSP_AV1_AGGREGATION_HEADER_SIZE 1

/**
 * Maximum number of bytes that an unsigned 64-bit value can occupy when
 * LEB128-encoded (used for OBU element lengths in AV1 RTP §4.5 and for
 * obu_size inside AV1 OBU headers).
 */
#define SMOLRTSP_AV1_LEB128_MAX_SIZE 10

/**
 * AV1 OBU types (AV1 spec §6.2.1).
 */
#define SMOLRTSP_AV1_OBU_RESERVED_0             0
#define SMOLRTSP_AV1_OBU_SEQUENCE_HEADER        1
#define SMOLRTSP_AV1_OBU_TEMPORAL_DELIMITER     2
#define SMOLRTSP_AV1_OBU_FRAME_HEADER           3
#define SMOLRTSP_AV1_OBU_TILE_GROUP             4
#define SMOLRTSP_AV1_OBU_METADATA               5
#define SMOLRTSP_AV1_OBU_FRAME                  6
#define SMOLRTSP_AV1_OBU_REDUNDANT_FRAME_HEADER 7
#define SMOLRTSP_AV1_OBU_TILE_LIST              8
#define SMOLRTSP_AV1_OBU_PADDING                15

/**
 * The AV1 RTP aggregation header byte (AOMedia AV1 RTP §4.4).
 *
 * Bit layout (network byte order, bit 0 is the MSB):
 *
 *      0 1 2 3 4 5 6 7
 *     +-+-+-+-+-+-+-+-+
 *     |Z|Y| W |N|-|-|-|
 *     +-+-+-+-+-+-+-+-+
 */
typedef struct {
    /** Z -- the first OBU element of this packet is the continuation of
     *  an OBU fragment that started in the previous packet. */
    bool z;
    /** Y -- the last OBU element of this packet is an OBU fragment that
     *  will continue in the next packet. */
    bool y;
    /** W (2 bits) -- count of OBU elements in this packet. 0 means each
     *  element is preceded by a LEB128 length field; 1, 2, or 3 means
     *  exactly that many elements and the last one omits its length
     *  field. */
    uint8_t w;
    /** N -- this packet is the first packet of a new coded video
     *  sequence. RFC constraint: if N == 1 then Z must be 0. */
    bool n;
} SmolRTSP_Av1AggregationHeader;

/**
 * Serializes @p self into the single byte at @p buffer (network order).
 */
void SmolRTSP_Av1AggregationHeader_serialize(
    SmolRTSP_Av1AggregationHeader self, uint8_t buffer[restrict]);

/**
 * Encodes @p value as a LEB128 unsigned varint into @p buffer.
 *
 * @param[in] value The value to encode.
 * @param[out] buffer Output buffer; must hold at least
 *                    #SMOLRTSP_AV1_LEB128_MAX_SIZE bytes.
 *
 * @return The number of bytes written (in `[1, SMOLRTSP_AV1_LEB128_MAX_SIZE]`).
 */
size_t smolrtsp_av1_leb128_encode(uint64_t value, uint8_t buffer[restrict])
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Decodes one LEB128 unsigned varint from @p buffer.
 *
 * @param[in] buffer Input buffer.
 * @param[in] buffer_len Number of bytes available at @p buffer.
 * @param[out] value Decoded value (only written on success).
 *
 * @return The number of bytes consumed (in `[1,
 *         SMOLRTSP_AV1_LEB128_MAX_SIZE]`), or 0 if the input was
 *         truncated or malformed.
 */
size_t smolrtsp_av1_leb128_decode(
    const uint8_t *buffer, size_t buffer_len,
    uint64_t *value) SMOLRTSP_PRIV_MUST_USE;

/**
 * Returns the number of bytes that #smolrtsp_av1_leb128_encode would
 * write for @p value.
 */
size_t smolrtsp_av1_leb128_size(uint64_t value) SMOLRTSP_PRIV_MUST_USE;

/**
 * Extracts the AV1 OBU type (bits 1-4 of the OBU header byte, AV1 spec
 * §5.3.2).
 */
static inline uint8_t smolrtsp_av1_obu_type(uint8_t header_byte) {
    return (header_byte >> 3) & 0x0Fu;
}

/**
 * Returns whether the AV1 OBU header byte's extension_flag is set
 * (bit 5 of the OBU header byte, AV1 spec §5.3.2).
 */
static inline bool smolrtsp_av1_obu_has_extension(uint8_t header_byte) {
    return ((header_byte >> 2) & 0x01u) != 0;
}

/**
 * Returns whether the AV1 OBU header byte's has_size_field flag is set
 * (bit 6 of the OBU header byte, AV1 spec §5.3.2).
 */
static inline bool smolrtsp_av1_obu_has_size_field(uint8_t header_byte) {
    return ((header_byte >> 1) & 0x01u) != 0;
}

/**
 * Returns @p header_byte with the has_size_field bit cleared. AOMedia
 * AV1 RTP §4.5 recommends clearing this bit on the wire because the
 * RTP-level LEB128 length prefix (or last-element length inference)
 * replaces obu_size.
 */
static inline uint8_t smolrtsp_av1_obu_clear_size_field(uint8_t header_byte) {
    return (uint8_t)(header_byte & ~0x02u);
}
