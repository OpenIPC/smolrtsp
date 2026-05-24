#include <smolrtsp/jpegxs.h>

void SmolRTSP_JpegXsPayloadHeader_serialize(
    SmolRTSP_JpegXsPayloadHeader self, uint8_t buffer[restrict]) {
    /* RFC 9134 §4.3 bit layout, MSB-first within a 32-bit big-endian word:
     *   bit  0     -- T              (1)
     *   bit  1     -- K              (1)
     *   bit  2     -- L              (1)
     *   bits 3-4   -- I              (2)
     *   bits 5-9   -- F counter      (5)
     *   bits 10-20 -- SEP counter    (11)
     *   bits 21-31 -- P counter      (11) */
    const uint32_t word = ((uint32_t)(self.sequential ? 1u : 0u) << 31) |
                          ((uint32_t)(self.pack_mode & 1u) << 30) |
                          ((uint32_t)(self.last_in_unit ? 1u : 0u) << 29) |
                          (((uint32_t)self.interlace & 0x3u) << 27) |
                          (((uint32_t)self.frame_counter & 0x1Fu) << 22) |
                          (((uint32_t)self.sep_counter & 0x7FFu) << 11) |
                          ((uint32_t)self.packet_counter & 0x7FFu);

    buffer[0] = (uint8_t)(word >> 24);
    buffer[1] = (uint8_t)(word >> 16);
    buffer[2] = (uint8_t)(word >> 8);
    buffer[3] = (uint8_t)(word);
}
