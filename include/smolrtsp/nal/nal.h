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

#define SMOLRTSP_NAL_HEADER_FU_HEADER_SIZE(h)                                                      \
    (MATCHES(h, SmolRTSP_NalHeader_H264) ? SMOLRTSP_H264_FU_HEADER_SIZE                            \
                                         : SMOLRTSP_H265_FU_HEADER_SIZE)

// clang-format off
datatype(
    SmolRTSP_NalHeader,
    (SmolRTSP_NalHeader_H264, SmolRTSP_H264NalHeader),
    (SmolRTSP_NalHeader_H265, SmolRTSP_H265NalHeader)
);
// clang-format on

typedef enum {
    SmolRTSP_NalCodec_H264,
    SmolRTSP_NalCodec_H265,
} SmolRTSP_NalCodec;

uint8_t SmolRTSP_NalHeader_unit_type(SmolRTSP_NalHeader h);

size_t SmolRTSP_NalHeader_size(SmolRTSP_NalHeader h);

bool SmolRTSP_NalHeader_is_vps(SmolRTSP_NalHeader h);
bool SmolRTSP_NalHeader_is_sps(SmolRTSP_NalHeader h);
bool SmolRTSP_NalHeader_is_pps(SmolRTSP_NalHeader h);
bool SmolRTSP_NalHeader_is_coded_slice_idr(SmolRTSP_NalHeader h);
bool SmolRTSP_NalHeader_is_coded_slice_non_idr(SmolRTSP_NalHeader h);

void SmolRTSP_NalHeader_write_fu_header(
    SmolRTSP_NalHeader h, uint8_t buffer[restrict], bool is_first_fragment, bool is_last_fragment);

// clang-format off
datatype(
    SmolRTSP_NalUnit,
    (SmolRTSP_NalUnit_H264, SmolRTSP_H264NalUnit),
    (SmolRTSP_NalUnit_H265, SmolRTSP_H265NalUnit)
);
// clang-format on

SmolRTSP_NalUnit SmolRTSP_NalUnit_new(SmolRTSP_NalHeader h, U8Slice99 payload);
SmolRTSP_NalHeader SmolRTSP_NalUnit_header(SmolRTSP_NalUnit self);
U8Slice99 SmolRTSP_NalUnit_payload(SmolRTSP_NalUnit self);

uint8_t nal_fu_header(bool is_first_fragment, bool is_last_fragment, uint8_t unit_type);

/**
 * If @p data contains a start code sequence, returns `true` and assigns @p
 * data_without_start_code, `false` otherwise.
 */
bool nal_test_start_code(U8Slice99 data, U8Slice99 *restrict data_without_start_code);
