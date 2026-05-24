#include <smolrtsp/nal/h266.h>

#include <smolrtsp/nal.h>

#include <string.h>

SmolRTSP_H266NalHeader
SmolRTSP_H266NalHeader_parse(uint8_t bytes[restrict static 2]) {
    /* RFC 9328 §1.1.4 / H.266 §7.3.1.2:
     * byte 0: F(1) | Z(1) | LayerId(6)
     * byte 1: Type(5) | TID(3) */
    return (SmolRTSP_H266NalHeader){
        .forbidden_zero_bit = (bytes[0] & 0b10000000) >> 7,
        .nuh_reserved_zero_bit = (bytes[0] & 0b01000000) >> 6,
        .nuh_layer_id = (bytes[0] & 0b00111111),
        .unit_type = (bytes[1] & 0b11111000) >> 3,
        .nuh_temporal_id_plus1 = (bytes[1] & 0b00000111),
    };
}

uint16_t SmolRTSP_H266NalHeader_serialize(SmolRTSP_H266NalHeader self) {
    const uint8_t bytes[2] = {
        [0] = (((uint8_t)self.forbidden_zero_bit & 0b00000001) << 7) |
              (((uint8_t)self.nuh_reserved_zero_bit & 0b00000001) << 6) |
              (self.nuh_layer_id & 0b00111111),

        [1] = ((self.unit_type & 0b00011111) << 3) |
              (self.nuh_temporal_id_plus1 & 0b00000111),
    };

    uint16_t n;
    memcpy(&n, bytes, sizeof bytes);
    return n;
}

bool SmolRTSP_H266NalHeader_is_vps(SmolRTSP_H266NalHeader self) {
    return SMOLRTSP_H266_NAL_UNIT_VPS_NUT == self.unit_type;
}

bool SmolRTSP_H266NalHeader_is_sps(SmolRTSP_H266NalHeader self) {
    return SMOLRTSP_H266_NAL_UNIT_SPS_NUT == self.unit_type;
}

bool SmolRTSP_H266NalHeader_is_pps(SmolRTSP_H266NalHeader self) {
    return SMOLRTSP_H266_NAL_UNIT_PPS_NUT == self.unit_type;
}

bool SmolRTSP_H266NalHeader_is_coded_slice_idr(SmolRTSP_H266NalHeader self) {
    return SMOLRTSP_H266_NAL_UNIT_IDR_W_RADL == self.unit_type ||
           SMOLRTSP_H266_NAL_UNIT_IDR_N_LP == self.unit_type;
}

bool SmolRTSP_H266NalHeader_is_coded_slice_non_idr(
    SmolRTSP_H266NalHeader self) {
    switch (self.unit_type) {
    case SMOLRTSP_H266_NAL_UNIT_TRAIL_NUT:
    case SMOLRTSP_H266_NAL_UNIT_STSA_NUT:
    case SMOLRTSP_H266_NAL_UNIT_RADL_NUT:
    case SMOLRTSP_H266_NAL_UNIT_RASL_NUT:
    case SMOLRTSP_H266_NAL_UNIT_CRA_NUT:
    case SMOLRTSP_H266_NAL_UNIT_GDR_NUT:
        return true;
    default:
        return false;
    }
}

void SmolRTSP_H266NalHeader_write_fu_header(
    SmolRTSP_H266NalHeader self, uint8_t buffer[restrict],
    bool is_first_fragment, bool is_last_fragment) {
    /* RFC 9328 §4.3.3 FU payload header: same 2-byte NAL header layout
     * with nal_unit_type rewritten to 29 (FU). */
    const uint16_t payload_hdr =
        SmolRTSP_H266NalHeader_serialize((SmolRTSP_H266NalHeader){
            .forbidden_zero_bit = self.forbidden_zero_bit,
            .nuh_reserved_zero_bit = self.nuh_reserved_zero_bit,
            .nuh_layer_id = self.nuh_layer_id,
            .unit_type = SMOLRTSP_H266_NAL_UNIT_FU,
            .nuh_temporal_id_plus1 = self.nuh_temporal_id_plus1,
        });

    /* RFC 9328 §4.3.3 FU header byte: S | E | P | FuType (5 bits).
     * Reuses smolrtsp_nal_fu_header (S|E|Type5) -- the P bit is set
     * only on the last byte of the last VCL NAL unit of a coded
     * picture; the conservative choice is to leave it 0 here, matching
     * how H.264/H.265 work. Producers that want to signal P can do so
     * via the higher-level transport's is_au_end and a future hook. */
    const uint8_t fu_header = smolrtsp_nal_fu_header(
        is_first_fragment, is_last_fragment, self.unit_type);

    buffer = SLICE99_APPEND(buffer, payload_hdr);
    buffer = SLICE99_APPEND(buffer, fu_header);
}
