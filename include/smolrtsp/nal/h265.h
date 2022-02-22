#pragma once

#include <stdbool.h>

#include <slice99.h>

// H.265 spec: <https://www.itu.int/rec/T-REC-H.265-201911-I/en>.

#define SMOLRTSP_H265_NAL_UNIT_TRAIL_N        0
#define SMOLRTSP_H265_NAL_UNIT_TRAIL_R        1
#define SMOLRTSP_H265_NAL_UNIT_TSA_N          2
#define SMOLRTSP_H265_NAL_UNIT_TSA_R          3
#define SMOLRTSP_H265_NAL_UNIT_STSA_N         4
#define SMOLRTSP_H265_NAL_UNIT_STSA_R         5
#define SMOLRTSP_H265_NAL_UNIT_RADL_N         6
#define SMOLRTSP_H265_NAL_UNIT_RADL_R         7
#define SMOLRTSP_H265_NAL_UNIT_RASL_N         8
#define SMOLRTSP_H265_NAL_UNIT_RASL_R         9
#define SMOLRTSP_H265_NAL_UNIT_BLA_W_LP       16
#define SMOLRTSP_H265_NAL_UNIT_BLA_W_RADL     17
#define SMOLRTSP_H265_NAL_UNIT_BLA_N_LP       18
#define SMOLRTSP_H265_NAL_UNIT_IDR_W_RADL     19
#define SMOLRTSP_H265_NAL_UNIT_IDR_N_LP       20
#define SMOLRTSP_H265_NAL_UNIT_CRA_NUT        21
#define SMOLRTSP_H265_NAL_UNIT_VPS_NUT        32
#define SMOLRTSP_H265_NAL_UNIT_SPS_NUT        33
#define SMOLRTSP_H265_NAL_UNIT_PPS_NUT        34
#define SMOLRTSP_H265_NAL_UNIT_AUD_NUT        35
#define SMOLRTSP_H265_NAL_UNIT_EOS_NUT        36
#define SMOLRTSP_H265_NAL_UNIT_EOB_NUT        37
#define SMOLRTSP_H265_NAL_UNIT_FD_NUT         38
#define SMOLRTSP_H265_NAL_UNIT_PREFIX_SEI_NUT 39
#define SMOLRTSP_H265_NAL_UNIT_SUFFIX_SEI_NUT 40

#define SMOLRTSP_H265_NAL_HEADER_SIZE sizeof(uint16_t)
#define SMOLRTSP_H265_FU_HEADER_SIZE                                                               \
    (/* payload-hdr */ sizeof(uint16_t) + /* fu-header */ +sizeof(uint8_t))

typedef struct {
    bool forbidden_zero_bit;       // f(1)
    uint8_t unit_type;             // u(6)
    uint8_t nuh_layer_id;          // u(6)
    uint8_t nuh_temporal_id_plus1; // u(3)
} SmolRTSP_H265NalHeader;

typedef struct {
    SmolRTSP_H265NalHeader header;
    U8Slice99 payload;
} SmolRTSP_H265NalUnit;

SmolRTSP_H265NalHeader SmolRTSP_H265NalHeader_parse(uint8_t bytes[restrict static 2]);
uint16_t SmolRTSP_H265NalHeader_serialize(SmolRTSP_H265NalHeader self);

bool SmolRTSP_H265NalHeader_is_vps(SmolRTSP_H265NalHeader self);
bool SmolRTSP_H265NalHeader_is_sps(SmolRTSP_H265NalHeader self);
bool SmolRTSP_H265NalHeader_is_pps(SmolRTSP_H265NalHeader self);
bool SmolRTSP_H265NalHeader_is_coded_slice_idr(SmolRTSP_H265NalHeader self);
bool SmolRTSP_H265NalHeader_is_coded_slice_non_idr(SmolRTSP_H265NalHeader self);

void SmolRTSP_H265NalHeader_write_fu_header(
    SmolRTSP_H265NalHeader h, uint8_t buffer[restrict], bool is_first_fragment,
    bool is_last_fragment);
