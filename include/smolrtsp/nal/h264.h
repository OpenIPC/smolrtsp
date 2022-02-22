#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <slice99.h>

// Information:
// <https://yumichan.net/video-processing/video-compression/introduction-to-h264-nal-unit/>.
// H.264 spec: <https://www.itu.int/rec/T-REC-H.264-201906-I/en>.

#define SMOLRTSP_H264_NAL_UNIT_UNSPECIFIED                  0
#define SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_NON_IDR          1
#define SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_DATA_PARTITION_A 2
#define SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_DATA_PARTITION_B 3
#define SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_DATA_PARTITION_C 4
#define SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_IDR              5
#define SMOLRTSP_H264_NAL_UNIT_SEI                          6
#define SMOLRTSP_H264_NAL_UNIT_SPS                          7
#define SMOLRTSP_H264_NAL_UNIT_PPS                          8
#define SMOLRTSP_H264_NAL_UNIT_AUD                          9
#define SMOLRTSP_H264_NAL_UNIT_END_OF_SEQUENCE              10
#define SMOLRTSP_H264_NAL_UNIT_END_OF_STREAM                11
#define SMOLRTSP_H264_NAL_UNIT_FILLER                       12
#define SMOLRTSP_H264_NAL_UNIT_SPS_EXT                      13
#define SMOLRTSP_H264_NAL_UNIT_PREFIX                       14
#define SMOLRTSP_H264_NAL_UNIT_SUBSET_SPS                   15
#define SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_AUX              19
#define SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_EXT              20
#define SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_EXT_DEPTH_VIEW   21

#define SMOLRTSP_H264_NAL_HEADER_SIZE sizeof(uint8_t)
#define SMOLRTSP_H264_FU_HEADER_SIZE                                                               \
    (/* fu-identifier */ sizeof(uint8_t) + /* fu-header */ sizeof(uint8_t))

typedef struct {
    bool forbidden_zero_bit; // f(1)
    uint8_t ref_idc;         // u(2)
    uint8_t unit_type;       // u(5)
} SmolRTSP_H264NalHeader;

typedef struct {
    SmolRTSP_H264NalHeader header;
    U8Slice99 payload;
} SmolRTSP_H264NalUnit;

SmolRTSP_H264NalHeader SmolRTSP_H264NalHeader_parse(uint8_t byte_header);

bool SmolRTSP_H264NalHeader_is_vps(SmolRTSP_H264NalHeader self);
bool SmolRTSP_H264NalHeader_is_sps(SmolRTSP_H264NalHeader self);
bool SmolRTSP_H264NalHeader_is_pps(SmolRTSP_H264NalHeader self);
bool SmolRTSP_H264NalHeader_is_coded_slice_idr(SmolRTSP_H264NalHeader self);
bool SmolRTSP_H264NalHeader_is_coded_slice_non_idr(SmolRTSP_H264NalHeader self);

void SmolRTSP_H264NalHeader_write_fu_header(
    SmolRTSP_H264NalHeader h, uint8_t buffer[restrict], bool is_first_fragment,
    bool is_last_fragment);
