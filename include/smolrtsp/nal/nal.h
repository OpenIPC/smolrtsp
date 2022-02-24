/**
 * @file
 * @brief A generic [NAL (Network Abstraction
 * Layer)](https://en.wikipedia.org/wiki/Network_Abstraction_Layer)
 * representation.
 */

#pragma once

#include <smolrtsp/nal/h264.h>
#include <smolrtsp/nal/h265.h>

#include <stdbool.h>
#include <stdint.h>

#include <datatype99.h>
#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * Destructs @p self into its header and its payload data.
 *
 * This macro expands to two variables named @p h and @p payload and initialises
 * them to `SmolRTSP_NalUnit_header(self)` and `SmolRTSP_NalUnit_payload(self)`,
 * respectively.
 */
#define SMOLRTSP_NAL_UNIT_DESTRUCT(self, h, payload)                           \
    SmolRTSP_NalHeader h = SmolRTSP_NalUnit_header(self);                      \
    U8Slice99 payload = SmolRTSP_NalUnit_payload(self)

/**
 * A generic NAL header (either H.264 or H.265).
 *
 * See [Datatype99](https://github.com/Hirrolot/datatype99) for the macro usage.
 */

// clang-format off
datatype99(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H264, SmolRTSP_H264NalHeader),
    (SmolRTSP_NalHeader_H265, SmolRTSP_H265NalHeader)
);
// clang-format on

/**
 * Returns the NAL unit type of @p self.
 */
uint8_t
SmolRTSP_NalHeader_unit_type(SmolRTSP_NalHeader self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Computes the size of @p self in bytes.
 */
size_t SmolRTSP_NalHeader_size(SmolRTSP_NalHeader self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Computes the size of a fragmentation unit (FU) header @p self.
 *
 * @see H.264 Fragmentation Units (FUs):
 * <https://datatracker.ietf.org/doc/html/rfc6184#section-5.8>
 * @see H.265 Fragmentation Units (FUs):
 * <https://datatracker.ietf.org/doc/html/rfc7798#section-4.4.3>
 */
size_t
SmolRTSP_NalHeader_fu_size(SmolRTSP_NalHeader self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Returns the RTP clock rate in kHz of @p self.
 */
uint32_t SmolRTSP_NalHeader_clock_rate_kHz(SmolRTSP_NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is VPS.
 */
bool SmolRTSP_NalHeader_is_vps(SmolRTSP_NalHeader self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is SPS.
 */
bool SmolRTSP_NalHeader_is_sps(SmolRTSP_NalHeader self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is PPS.
 */
bool SmolRTSP_NalHeader_is_pps(SmolRTSP_NalHeader self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is a coded slice IDR.
 */
bool SmolRTSP_NalHeader_is_coded_slice_idr(SmolRTSP_NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Checks whether @p self is a coded slice non-IDR.
 */
bool SmolRTSP_NalHeader_is_coded_slice_non_idr(SmolRTSP_NalHeader self)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Writes the binary representation of @p self into @p buffer.
 *
 * @param[in] self The header to write.
 * @param[out] buffer The memory area capable of storing
 * `SmolRTSP_NalHeader_size(self)` bytes.
 */
void SmolRTSP_NalHeader_serialize(
    SmolRTSP_NalHeader self, uint8_t buffer[restrict]);

/**
 * Writes a FU header of @p self to @p buffer.
 *
 * @param[in] self The header to write.
 * @param[out] buffer The memory area capable of storing
 * `SmolRTSP_NalHeader_fu_size(self)` bytes.
 * @param[in] is_first_fragment The indication of a start of the FU.
 * @param[in] is_last_fragment The indication of an end of the FU.
 *
 * @see H.264 Fragmentation Units (FUs):
 * <https://datatracker.ietf.org/doc/html/rfc6184#section-5.8>
 * @see H.265 Fragmentation Units (FUs):
 * <https://datatracker.ietf.org/doc/html/rfc7798#section-4.4.3>
 */
void SmolRTSP_NalHeader_write_fu_header(
    SmolRTSP_NalHeader self, uint8_t buffer[restrict], bool is_first_fragment,
    bool is_last_fragment);

/**
 * A generic NAL unit (either H.264 or H.265).
 *
 * See [Datatype99](https://github.com/Hirrolot/datatype99) for the macro usage.
 */

// clang-format off
datatype99(
    SmolRTSP_NalUnit,
    (SmolRTSP_NalUnit_H264, SmolRTSP_H264NalUnit),
    (SmolRTSP_NalUnit_H265, SmolRTSP_H265NalUnit)
);
// clang-format on

/**
 * Constructs a NAL unit from a header and payload data.
 */
SmolRTSP_NalUnit SmolRTSP_NalUnit_new(SmolRTSP_NalHeader h, U8Slice99 payload)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Returns the header of @p self.
 */
SmolRTSP_NalHeader
SmolRTSP_NalUnit_header(SmolRTSP_NalUnit self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Returns the payload data of @p self.
 */
U8Slice99
SmolRTSP_NalUnit_payload(SmolRTSP_NalUnit self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Creates a generic NAL FU header.
 */
uint8_t smolrtsp_nal_fu_header(
    bool is_first_fragment, bool is_last_fragment,
    uint8_t unit_type) SMOLRTSP_PRIV_MUST_USE;

/**
 * Computes the length of a start code sequence in @p data, which is either
 * `0x000001` or `0x00000001`.
 *
 * If there is no start code in @p data, the result is 0.
 */
ssize_t smolrtsp_nal_test_start_code(U8Slice99 data) SMOLRTSP_PRIV_MUST_USE;
