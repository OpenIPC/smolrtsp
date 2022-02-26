#include <smolrtsp/nal/h264.h>

#include <smolrtsp/nal.h>

SmolRTSP_H264NalHeader SmolRTSP_H264NalHeader_parse(uint8_t byte_header) {
    return (SmolRTSP_H264NalHeader){
        .forbidden_zero_bit = (byte_header & 0b10000000) >> 7 == 1,
        .ref_idc = (byte_header & 0b01100000) >> 5,
        .unit_type = (byte_header & 0b00011111),
    };
}

uint8_t SmolRTSP_H264NalHeader_serialize(SmolRTSP_H264NalHeader self) {
    return (self.forbidden_zero_bit ? 0b10000000 : 0b00000000) |
           (self.ref_idc << 5) | (self.unit_type);
}

bool SmolRTSP_H264NalHeader_is_vps(SmolRTSP_H264NalHeader self) {
    (void)self;
    return false;
}

bool SmolRTSP_H264NalHeader_is_sps(SmolRTSP_H264NalHeader self) {
    return SMOLRTSP_H264_NAL_UNIT_SPS == self.unit_type;
}

bool SmolRTSP_H264NalHeader_is_pps(SmolRTSP_H264NalHeader self) {
    return SMOLRTSP_H264_NAL_UNIT_PPS == self.unit_type;
}

bool SmolRTSP_H264NalHeader_is_coded_slice_idr(SmolRTSP_H264NalHeader self) {
    return SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_IDR == self.unit_type;
}

bool SmolRTSP_H264NalHeader_is_coded_slice_non_idr(
    SmolRTSP_H264NalHeader self) {
    return SMOLRTSP_H264_NAL_UNIT_CODED_SLICE_NON_IDR == self.unit_type;
}

void SmolRTSP_H264NalHeader_write_fu_header(
    SmolRTSP_H264NalHeader self, uint8_t buffer[restrict],
    bool is_first_fragment, bool is_last_fragment) {
    uint8_t fu_identifier = (uint8_t)0b01111100; // 0, nal_ref_idc, FU-A (28)

    if ((self.ref_idc & 0b00000010) == 0) {
        fu_identifier &= 0b00111111;
    }
    if ((self.ref_idc & 0b00000001) == 0) {
        fu_identifier &= 0b01011111;
    }

    const uint8_t fu_header = smolrtsp_nal_fu_header(
        is_first_fragment, is_last_fragment, self.unit_type);

    buffer = SLICE99_APPEND(buffer, fu_identifier);
    buffer = SLICE99_APPEND(buffer, fu_header);
}
