/**
 * @file
 * @brief A generic [NAL (Network Abstraction
 * Layer)](https://en.wikipedia.org/wiki/Network_Abstraction_Layer) representation.
 */

#pragma once

#include <smolrtsp/nal/h264.h>
#include <smolrtsp/nal/h265.h>

#include <stdbool.h>
#include <stdint.h>

#include <datatype99.h>
#include <slice99.h>

#define SMOLRTSP_NAL_UNIT_DESTRUCT(nal_unit, h, payload)                                           \
    SmolRTSP_NalHeader h = SmolRTSP_NalUnit_header(nal_unit);                                      \
    U8Slice99 payload = SmolRTSP_NalUnit_payload(nal_unit)

/**
 * The size of a fragmentation unit (FU) header @p h.
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc6184#section-5.8> (H.264)
 * @see <https://datatracker.ietf.org/doc/html/rfc7798#section-4.4.3> (H.265)
 */
#define SMOLRTSP_NAL_HEADER_FU_HEADER_SIZE(h)                                                      \
    (MATCHES99((h), SmolRTSP_NalHeader_H264) ? SMOLRTSP_H264_FU_HEADER_SIZE                        \
                                             : SMOLRTSP_H265_FU_HEADER_SIZE)

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
uint8_t SmolRTSP_NalHeader_unit_type(SmolRTSP_NalHeader self);

/**
 * Computes the size of @p self in bytes.
 */
size_t SmolRTSP_NalHeader_size(SmolRTSP_NalHeader self);

/**
 * Checks whether @p self is VPS.
 */
bool SmolRTSP_NalHeader_is_vps(SmolRTSP_NalHeader self);

/**
 * Checks whether @p self is SPS.
 */
bool SmolRTSP_NalHeader_is_sps(SmolRTSP_NalHeader self);

/**
 * Checks whether @p self is PPS.
 */
bool SmolRTSP_NalHeader_is_pps(SmolRTSP_NalHeader self);

/**
 * Checks whether @p self is a coded slice IDR.
 */
bool SmolRTSP_NalHeader_is_coded_slice_idr(SmolRTSP_NalHeader self);

/**
 * Checks whether @p self is a coded slice non-IDR.
 */
bool SmolRTSP_NalHeader_is_coded_slice_non_idr(SmolRTSP_NalHeader self);

/**
 * Writes a FU header of @p self to @p buffer.
 *
 * @param[in] self The header to write.
 * @param[out] buffer The memory area capable of storing `SMOLRTSP_NAL_HEADER_FU_HEADER_SIZE(self)`
 * bytes.
 * @param[in] is_first_fragment The indication of a start of the FU.
 * @param[in] is_last_fragment The indication of an end of the FU.
 *
 * @see <https://datatracker.ietf.org/doc/html/rfc6184#section-5.8> (H.264)
 * @see <https://datatracker.ietf.org/doc/html/rfc7798#section-4.4.3> (H.265)
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
SmolRTSP_NalUnit SmolRTSP_NalUnit_new(SmolRTSP_NalHeader h, U8Slice99 payload);

/**
 * Returns the header of @p self.
 */
SmolRTSP_NalHeader SmolRTSP_NalUnit_header(SmolRTSP_NalUnit self);

/**
 * Returns the payload data of @p self.
 */
U8Slice99 SmolRTSP_NalUnit_payload(SmolRTSP_NalUnit self);

/**
 * Creates a generic NAL FU header.
 */
uint8_t smolrtsp_nal_fu_header(bool is_first_fragment, bool is_last_fragment, uint8_t unit_type);

/**
 * If @p data contains a start code sequence, returns `true` and assigns @p
 * data_without_start_code, `false` otherwise.
 */
bool smolrtsp_nal_test_start_code(U8Slice99 data, U8Slice99 *restrict data_without_start_code);
