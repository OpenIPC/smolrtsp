#include <smolrtsp/nal/h265.h>

#include <smolrtsp/nal.h>

#include <arpa/inet.h>
#include <string.h>

SmolRTSP_H265NalHeader
SmolRTSP_H265NalHeader_parse(uint8_t bytes[restrict static 2]) {
    return (SmolRTSP_H265NalHeader){
        .forbidden_zero_bit = (bytes[0] & 0b10000000) >> 7,
        .unit_type = (bytes[0] & 0b01111110) >> 1,
        .nuh_layer_id =
            ((bytes[0] & 0b00000001) << 5) | ((bytes[1] & 0b11111000) >> 3),
        .nuh_temporal_id_plus1 = (bytes[1] & 0b00000111),
    };
}

uint16_t SmolRTSP_H265NalHeader_serialize(SmolRTSP_H265NalHeader self) {
    const uint8_t bytes[2] = {
        [0] = (((uint8_t)self.forbidden_zero_bit & 0b00000001) << 7) |
              ((self.unit_type & 0b00111111) << 1) |
              ((self.nuh_layer_id & 0b00100000) >> 5),

        [1] = ((self.nuh_layer_id & 0b00011111) << 3) |
              (self.nuh_temporal_id_plus1 & 0b00000111),
    };

    uint16_t n;
    memcpy(&n, bytes, sizeof bytes);
    return n;
}

bool SmolRTSP_H265NalHeader_is_vps(SmolRTSP_H265NalHeader self) {
    return SMOLRTSP_H265_NAL_UNIT_VPS_NUT == self.unit_type;
}

bool SmolRTSP_H265NalHeader_is_sps(SmolRTSP_H265NalHeader self) {
    return SMOLRTSP_H265_NAL_UNIT_SPS_NUT == self.unit_type;
}

bool SmolRTSP_H265NalHeader_is_pps(SmolRTSP_H265NalHeader self) {
    return SMOLRTSP_H265_NAL_UNIT_PPS_NUT == self.unit_type;
}

bool SmolRTSP_H265NalHeader_is_coded_slice_idr(SmolRTSP_H265NalHeader self) {
    return SMOLRTSP_H265_NAL_UNIT_IDR_W_RADL == self.unit_type;
}

bool SmolRTSP_H265NalHeader_is_coded_slice_non_idr(
    SmolRTSP_H265NalHeader self) {
    return SMOLRTSP_H265_NAL_UNIT_IDR_N_LP == self.unit_type ||
           SMOLRTSP_H265_NAL_UNIT_TRAIL_R == self.unit_type;
}

void SmolRTSP_H265NalHeader_write_fu_header(
    SmolRTSP_H265NalHeader self, uint8_t buffer[restrict],
    bool is_first_fragment, bool is_last_fragment) {
    const uint16_t payload_hdr =
        SmolRTSP_H265NalHeader_serialize((SmolRTSP_H265NalHeader){
            .forbidden_zero_bit = self.forbidden_zero_bit,
            .unit_type = 49,
            .nuh_layer_id = self.nuh_layer_id,
            .nuh_temporal_id_plus1 = self.nuh_temporal_id_plus1,
        });

    const uint8_t fu_header = smolrtsp_nal_fu_header(
        is_first_fragment, is_last_fragment, self.unit_type);

    buffer = SLICE99_APPEND(buffer, payload_hdr);
    buffer = SLICE99_APPEND(buffer, fu_header);
}
