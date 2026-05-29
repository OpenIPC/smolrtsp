/**
 * @file
 * @brief A generic [NAL (Network Abstraction
 * Layer)](https://en.wikipedia.org/wiki/Network_Abstraction_Layer)
 * representation.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <datatype99.h>
#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

#if !defined(SMOLRTSP_WITH_H264) && !defined(SMOLRTSP_WITH_H265) &&            \
    !defined(SMOLRTSP_WITH_H266)
#error "smolrtsp: at least one NAL codec must be enabled at build time."
#endif

#ifdef SMOLRTSP_WITH_H264
#include <smolrtsp/nal/h264.h>
#endif
#ifdef SMOLRTSP_WITH_H265
#include <smolrtsp/nal/h265.h>
#endif
#ifdef SMOLRTSP_WITH_H266
#include <smolrtsp/nal/h266.h>
#endif

/**
 * A generic NAL header. The set of variants carried by the sum type depends
 * on which of `SMOLRTSP_WITH_H264`, `SMOLRTSP_WITH_H265`, `SMOLRTSP_WITH_H266`
 * were enabled at build time (at least one must be).
 *
 * See [Datatype99](https://github.com/hirrolot/datatype99) for the macro usage.
 */

// clang-format off
// `datatype99` is variadic but its variant list cannot contain preprocessor
// directives, so the (up to 2**3 - 1 = 7) live combinations are enumerated.
#if defined(SMOLRTSP_WITH_H264) && defined(SMOLRTSP_WITH_H265) && defined(SMOLRTSP_WITH_H266)
datatype99(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H264, SmolRTSP_H264NalHeader),
    (SmolRTSP_NalHeader_H265, SmolRTSP_H265NalHeader),
    (SmolRTSP_NalHeader_H266, SmolRTSP_H266NalHeader)
);
#elif defined(SMOLRTSP_WITH_H264) && defined(SMOLRTSP_WITH_H265)
datatype99(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H264, SmolRTSP_H264NalHeader),
    (SmolRTSP_NalHeader_H265, SmolRTSP_H265NalHeader)
);
#elif defined(SMOLRTSP_WITH_H264) && defined(SMOLRTSP_WITH_H266)
datatype99(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H264, SmolRTSP_H264NalHeader),
    (SmolRTSP_NalHeader_H266, SmolRTSP_H266NalHeader)
);
#elif defined(SMOLRTSP_WITH_H265) && defined(SMOLRTSP_WITH_H266)
datatype99(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H265, SmolRTSP_H265NalHeader),
    (SmolRTSP_NalHeader_H266, SmolRTSP_H266NalHeader)
);
#elif defined(SMOLRTSP_WITH_H264)
datatype99(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H264, SmolRTSP_H264NalHeader)
);
#elif defined(SMOLRTSP_WITH_H265)
datatype99(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H265, SmolRTSP_H265NalHeader)
);
#elif defined(SMOLRTSP_WITH_H266)
datatype99(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H266, SmolRTSP_H266NalHeader)
);
#endif
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
 * A generic NAL unit (H.264, H.265, or H.266 / VVC).
 */
typedef struct {
    /**
     * The header of this NAL unit.
     */
    SmolRTSP_NalHeader header;

    /**
     * The payload data of this NAL unit (not including the header).
     */
    U8Slice99 payload;
} SmolRTSP_NalUnit;

/**
 * Creates a generic NAL FU header.
 */
uint8_t smolrtsp_nal_fu_header(
    bool is_first_fragment, bool is_last_fragment,
    uint8_t unit_type) SMOLRTSP_PRIV_MUST_USE;

/**
 * A function that tests whether @p data starts with some start code.
 *
 * @return The number of start code bytes. If there is no start code in the
 * beginning of @p data, returns 0.
 *
 * @see #smolrtsp_test_start_code_3b
 * @see #smolrtsp_test_start_code_4b
 */
typedef size_t (*SmolRTSP_NalStartCodeTester)(U8Slice99 data);

/**
 * Returns a start code tester for @p data.
 *
 * If @p data does not begin with a start code (either `0x000001` or
 * `0x00000001`), returns `NULL`.
 *
 * This function is useful when you have some `*.h264` bitstream file and you
 * want to determine what start code it uses -- just pass the beginning of this
 * file to #smolrtsp_determine_start_code and invoke the returned tester
 * multiple times afterwards.
 */
SmolRTSP_NalStartCodeTester
smolrtsp_determine_start_code(U8Slice99 data) SMOLRTSP_PRIV_MUST_USE;

/**
 * The 3-byte start code tester (`0x000001`).
 */
size_t smolrtsp_test_start_code_3b(U8Slice99 data);

/**
 * The 3-byte start code tester (`0x00000001`).
 */
size_t smolrtsp_test_start_code_4b(U8Slice99 data);
