/**
 * @file
 * @brief [H.265](https://en.wikipedia.org/wiki/High_Efficiency_Video_Coding)
 * NAL representation.
 *
 * @see H.265 spec: <https://www.itu.int/rec/T-REC-H.265>.
 */

#pragma once

#include <stdbool.h>

#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The size of an H.265 NAL header.
 */
#define SMOLRTSP_H265_NAL_HEADER_SIZE sizeof(uint16_t)

/**
 * The size of an H.265 fragmentation unit (FU) header.
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc7798#section-4.4.3>
 */
#define SMOLRTSP_H265_FU_HEADER_SIZE                                           \
    (/* payload-hdr */ sizeof(uint16_t) + /* fu-header */ sizeof(uint8_t))

/**
 * An H.265 NAL header.
 */
typedef struct {
    /**
     * `forbidden_zero_bit` f(1).
     */
    bool forbidden_zero_bit;

    /**
     * `unit_type` u(6).
     */
    uint8_t unit_type;

    /**
     * `nuh_layer_id` u(6).
     */
    uint8_t nuh_layer_id;

    /**
     * `nuh_temporal_id_plus1` u(3).
     */
    uint8_t nuh_temporal_id_plus1;
} SmolRTSP_H265NalHeader;

/**
 * An H.265 NAL unit.
 */
typedef struct {
    /**
     * The NAL header of this NAL unit.
     */
    SmolRTSP_H265NalHeader header;

    /**
     * The payload data of this NAL unit (not including the header).
     */
    U8Slice99 payload;
} SmolRTSP_H265NalUnit;

/**
 * Parses an H.265 NAL header from @p byte_header.
 */
SmolRTSP_H265NalHeader SmolRTSP_H265NalHeader_parse(
    uint8_t bytes[restrict static 2]) SMOLRTSP_PRIV_MUST_USE;

/**
 * Converts @p self to a two-octet representation.
 */
uint16_t SmolRTSP_H265NalHeader_serialize(SmolRTSP_H265NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is VPS.
 */
bool SmolRTSP_H265NalHeader_is_vps(SmolRTSP_H265NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is SPS.
 */
bool SmolRTSP_H265NalHeader_is_sps(SmolRTSP_H265NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is PPS.
 */
bool SmolRTSP_H265NalHeader_is_pps(SmolRTSP_H265NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is a coded slice IDR.
 */
bool SmolRTSP_H265NalHeader_is_coded_slice_idr(SmolRTSP_H265NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is a coded slice non-IDR.
 */
bool SmolRTSP_H265NalHeader_is_coded_slice_non_idr(SmolRTSP_H265NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Writes a FU header of @p self to @p buffer.
 *
 * @param[in] self The header to write.
 * @param[out] buffer The memory area capable of storing
 * #SMOLRTSP_H265_FU_HEADER_SIZE bytes.
 * @param[in] is_first_fragment The indication of a start of the FU.
 * @param[in] is_last_fragment The indication of an end of the FU.
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc7798#section-4.4.3>
 */
void SmolRTSP_H265NalHeader_write_fu_header(
    SmolRTSP_H265NalHeader self, uint8_t buffer[restrict],
    bool is_first_fragment, bool is_last_fragment);

/**
 * Coded slice segment of a non-TSA, non-STSA trailing picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_TRAIL_N 0

/**
 * Coded slice segment of a non-TSA, non-STSA trailing picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_TRAIL_R 1

/**
 * Coded slice segment of a TSA picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_TSA_N 2

/**
 * Coded slice segment of a TSA picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_TSA_R 3

/**
 * Coded slice segment of an STSA picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_STSA_N 4

/**
 * Coded slice segment of an STSA picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_STSA_R 5

/**
 * Coded slice segment of a RADL picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_RADL_N 6

/**
 * Coded slice segment of a RADL picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_RADL_R 7

/**
 * Coded slice segment of a RASL picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_RASL_N 8

/**
 * Coded slice segment of a RASL picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_RASL_R 9

/**
 * Coded slice segment of a BLA picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_BLA_W_LP 16

/**
 * Coded slice segment of a BLA picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_BLA_W_RADL 17

/**
 * Coded slice segment of a BLA picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_BLA_N_LP 18

/**
 * Coded slice segment of an IDR picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_IDR_W_RADL 19

/**
 * Coded slice segment of an IDR picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_IDR_N_LP 20

/**
 * Coded slice segment of a CRA picture.
 */
#define SMOLRTSP_H265_NAL_UNIT_CRA_NUT 21

/**
 * Video parameter set.
 */
#define SMOLRTSP_H265_NAL_UNIT_VPS_NUT 32

/**
 * Sequence parameter set.
 */
#define SMOLRTSP_H265_NAL_UNIT_SPS_NUT 33

/**
 * Picture parameter set.
 */
#define SMOLRTSP_H265_NAL_UNIT_PPS_NUT 34

/**
 * Access unit delimiter.
 */
#define SMOLRTSP_H265_NAL_UNIT_AUD_NUT 35

/**
 * End of sequence.
 */
#define SMOLRTSP_H265_NAL_UNIT_EOS_NUT 36

/**
 * End of bitstream.
 */
#define SMOLRTSP_H265_NAL_UNIT_EOB_NUT 37

/**
 * Filler data.
 */
#define SMOLRTSP_H265_NAL_UNIT_FD_NUT 38

/**
 * Supplemental enhancement information.
 */
#define SMOLRTSP_H265_NAL_UNIT_PREFIX_SEI_NUT 39

/**
 * Supplemental enhancement information.
 */
#define SMOLRTSP_H265_NAL_UNIT_SUFFIX_SEI_NUT 40
