#include <smolrtsp/nal/h265.h>

#include <smolrtsp/nal/nal.h>

#include <arpa/inet.h>

SmolRTSP_H265NalHeader
SmolRTSP_H265NalHeader_parse(uint8_t bytes[restrict static 2]) {
    SmolRTSP_H265NalHeader header;

    header.forbidden_zero_bit = (bytes[0] & 0b10000000) >> 7;
    header.unit_type = (bytes[0] & 0b01111110) >> 1;
    header.nuh_layer_id = (bytes[0] & 0b00000001) << 7;
    header.nuh_layer_id |= (bytes[1] & 0b11111000) >> 3;
    header.nuh_temporal_id_plus1 = (bytes[1] & 0b00000111);

    return header;
}

uint16_t SmolRTSP_H265NalHeader_serialize(SmolRTSP_H265NalHeader h) {
    union {
        uint16_t n;
        uint8_t bytes[2];
    } data = {.bytes = {0b11100010, 0b00000000}};

    if (!h.forbidden_zero_bit) {
        data.bytes[0] &= 0b01111111;
    }

    if ((h.nuh_layer_id & 0b00100000) >> 5 != 0) {
        data.bytes[0] |= 0b00000001;
    }

    data.bytes[1] |= (h.nuh_layer_id & 0b00011111) << 2;
    data.bytes[1] |= (h.nuh_temporal_id_plus1 & 0b00000111);

    return data.n;
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
