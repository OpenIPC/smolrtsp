/**
 * @file
 * @brief [H.266 / VVC](https://en.wikipedia.org/wiki/Versatile_Video_Coding)
 * NAL representation.
 *
 * @see H.266 spec: <https://www.itu.int/rec/T-REC-H.266>.
 * @see RTP payload format: <https://datatracker.ietf.org/doc/html/rfc9328>.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The size of an H.266 NAL header.
 */
#define SMOLRTSP_H266_NAL_HEADER_SIZE sizeof(uint16_t)

/**
 * The size of an H.266 fragmentation unit (FU) header.
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc9328#section-4.3.3>
 */
#define SMOLRTSP_H266_FU_HEADER_SIZE                                           \
    (/* payload-hdr */ sizeof(uint16_t) + /* fu-header */ sizeof(uint8_t))

/**
 * An H.266 NAL header (RFC 9328 §1.1.4 / VVC spec §7.3.1.2).
 *
 * Bit layout, transmitted in network byte order:
 *
 *     +---------------+---------------+
 *     |0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7|
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     |F|Z| LayerID   |  Type   | TID |
 *     +---------------+---------------+
 */
typedef struct {
    /**
     * `forbidden_zero_bit` f(1).
     */
    bool forbidden_zero_bit;

    /**
     * `nuh_reserved_zero_bit` f(1). MUST be 0 in valid VVC streams.
     */
    bool nuh_reserved_zero_bit;

    /**
     * `nuh_layer_id` u(6).
     */
    uint8_t nuh_layer_id;

    /**
     * `nal_unit_type` u(5).
     */
    uint8_t unit_type;

    /**
     * `nuh_temporal_id_plus1` u(3).
     */
    uint8_t nuh_temporal_id_plus1;
} SmolRTSP_H266NalHeader;

/**
 * An H.266 NAL unit.
 */
typedef struct {
    /**
     * The NAL header of this NAL unit.
     */
    SmolRTSP_H266NalHeader header;

    /**
     * The payload data of this NAL unit (not including the header).
     */
    U8Slice99 payload;
} SmolRTSP_H266NalUnit;

/**
 * Parses an H.266 NAL header from @p bytes.
 */
SmolRTSP_H266NalHeader SmolRTSP_H266NalHeader_parse(
    uint8_t bytes[restrict static 2]) SMOLRTSP_PRIV_MUST_USE;

/**
 * Converts @p self to a two-octet representation.
 */
uint16_t SmolRTSP_H266NalHeader_serialize(SmolRTSP_H266NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is VPS.
 */
bool SmolRTSP_H266NalHeader_is_vps(SmolRTSP_H266NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is SPS.
 */
bool SmolRTSP_H266NalHeader_is_sps(SmolRTSP_H266NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is PPS.
 */
bool SmolRTSP_H266NalHeader_is_pps(SmolRTSP_H266NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is a coded slice IDR (either IDR_W_RADL or
 * IDR_N_LP).
 */
bool SmolRTSP_H266NalHeader_is_coded_slice_idr(SmolRTSP_H266NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is a coded slice non-IDR (TRAIL_NUT, STSA_NUT,
 * RADL_NUT, RASL_NUT, CRA_NUT, or GDR_NUT).
 */
bool SmolRTSP_H266NalHeader_is_coded_slice_non_idr(SmolRTSP_H266NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Writes an FU payload header and FU header for @p self into @p buffer.
 *
 * @param[in] self The header to write.
 * @param[out] buffer The memory area capable of storing
 * #SMOLRTSP_H266_FU_HEADER_SIZE bytes.
 * @param[in] is_first_fragment The indication of a start of the FU.
 * @param[in] is_last_fragment The indication of an end of the FU.
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc9328#section-4.3.3>
 */
void SmolRTSP_H266NalHeader_write_fu_header(
    SmolRTSP_H266NalHeader self, uint8_t buffer[restrict],
    bool is_first_fragment, bool is_last_fragment);

/* --- VVC NAL unit types (H.266 spec §7.4.2.2 Table 5) ---------------- */

/** Coded slice of a non-STSA, non-RASL trailing picture (TRAIL). */
#define SMOLRTSP_H266_NAL_UNIT_TRAIL_NUT 0

/** Coded slice of a temporal sub-layer access (STSA) picture. */
#define SMOLRTSP_H266_NAL_UNIT_STSA_NUT 1

/** Coded slice of a random-access decodable leading (RADL) picture. */
#define SMOLRTSP_H266_NAL_UNIT_RADL_NUT 2

/** Coded slice of a random-access skipped leading (RASL) picture. */
#define SMOLRTSP_H266_NAL_UNIT_RASL_NUT 3

/** Coded slice of an IDR picture with associated RADL pictures. */
#define SMOLRTSP_H266_NAL_UNIT_IDR_W_RADL 7

/** Coded slice of an IDR picture with no leading pictures. */
#define SMOLRTSP_H266_NAL_UNIT_IDR_N_LP 8

/** Coded slice of a clean random-access (CRA) picture. */
#define SMOLRTSP_H266_NAL_UNIT_CRA_NUT 9

/** Coded slice of a gradual decoding refresh (GDR) picture. */
#define SMOLRTSP_H266_NAL_UNIT_GDR_NUT 10

/** Operating point information. */
#define SMOLRTSP_H266_NAL_UNIT_OPI_NUT 12

/** Decoding capability information. */
#define SMOLRTSP_H266_NAL_UNIT_DCI_NUT 13

/** Video parameter set. */
#define SMOLRTSP_H266_NAL_UNIT_VPS_NUT 14

/** Sequence parameter set. */
#define SMOLRTSP_H266_NAL_UNIT_SPS_NUT 15

/** Picture parameter set. */
#define SMOLRTSP_H266_NAL_UNIT_PPS_NUT 16

/** Prefix adaptation parameter set. */
#define SMOLRTSP_H266_NAL_UNIT_PREFIX_APS_NUT 17

/** Suffix adaptation parameter set. */
#define SMOLRTSP_H266_NAL_UNIT_SUFFIX_APS_NUT 18

/** Picture header. */
#define SMOLRTSP_H266_NAL_UNIT_PH_NUT 19

/** Access unit delimiter. */
#define SMOLRTSP_H266_NAL_UNIT_AUD_NUT 20

/** End of sequence. */
#define SMOLRTSP_H266_NAL_UNIT_EOS_NUT 21

/** End of bitstream. */
#define SMOLRTSP_H266_NAL_UNIT_EOB_NUT 22

/** Prefix supplemental enhancement information. */
#define SMOLRTSP_H266_NAL_UNIT_PREFIX_SEI_NUT 23

/** Suffix supplemental enhancement information. */
#define SMOLRTSP_H266_NAL_UNIT_SUFFIX_SEI_NUT 24

/** Filler data. */
#define SMOLRTSP_H266_NAL_UNIT_FD_NUT 25

/**
 * Aggregation Packet (RTP-only, RFC 9328 §4.3.2).
 * Carries two or more NAL units of one access unit in a single RTP
 * packet.
 */
#define SMOLRTSP_H266_NAL_UNIT_AP 28

/**
 * Fragmentation Unit (RTP-only, RFC 9328 §4.3.3).
 * Carries a fragment of a NAL unit that does not fit in one RTP packet.
 */
#define SMOLRTSP_H266_NAL_UNIT_FU 29
