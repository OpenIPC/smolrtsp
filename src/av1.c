#include <smolrtsp/av1.h>

void SmolRTSP_Av1AggregationHeader_serialize(
    SmolRTSP_Av1AggregationHeader self, uint8_t buffer[restrict]) {
    /* AOMedia AV1 RTP §4.4 aggregation header byte (bit 0 = MSB):
     *   bit 0:   Z (continuation start)
     *   bit 1:   Y (continuation end)
     *   bits 2-3: W (OBU element count, 0..3)
     *   bit 4:   N (new coded video sequence)
     *   bits 5-7: reserved, must be 0 */
    buffer[0] =
        (uint8_t)((self.z ? 0x80u : 0u) | (self.y ? 0x40u : 0u) |
                  ((uint32_t)(self.w & 0x3u) << 4) | (self.n ? 0x08u : 0u));
}

size_t smolrtsp_av1_leb128_encode(uint64_t value, uint8_t buffer[restrict]) {
    size_t n = 0;
    do {
        uint8_t byte = (uint8_t)(value & 0x7Fu);
        value >>= 7;
        if (value != 0) {
            byte |= 0x80u;
        }
        buffer[n++] = byte;
    } while (value != 0);
    return n;
}

size_t smolrtsp_av1_leb128_decode(
    const uint8_t *buffer, size_t buffer_len, uint64_t *value) {
    uint64_t acc = 0;
    size_t shift = 0;
    for (size_t i = 0; i < buffer_len && i < SMOLRTSP_AV1_LEB128_MAX_SIZE;
         i++) {
        const uint8_t byte = buffer[i];
        acc |= (uint64_t)(byte & 0x7Fu) << shift;
        if ((byte & 0x80u) == 0) {
            *value = acc;
            return i + 1;
        }
        shift += 7;
    }
    return 0; /* truncated or > LEB128_MAX_SIZE bytes without termination */
}

size_t smolrtsp_av1_leb128_size(uint64_t value) {
    size_t n = 0;
    do {
        value >>= 7;
        n++;
    } while (value != 0);
    return n;
}
