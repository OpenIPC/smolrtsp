#include <smolrtsp/jpeg.h>

void SmolRTSP_JpegPayloadHeader_serialize(
    SmolRTSP_JpegPayloadHeader self, uint8_t buffer[restrict]) {
    /* RFC 2435 §3.1 wire layout:
     *   byte 0     -- Type-Specific
     *   bytes 1..3 -- Fragment Offset (24-bit, big-endian)
     *   byte 4     -- Type
     *   byte 5     -- Q
     *   byte 6     -- Width  (units of 8 pixels)
     *   byte 7     -- Height (units of 8 pixels) */
    buffer[0] = self.type_specific;
    buffer[1] = (uint8_t)((self.fragment_offset >> 16) & 0xFFu);
    buffer[2] = (uint8_t)((self.fragment_offset >> 8) & 0xFFu);
    buffer[3] = (uint8_t)(self.fragment_offset & 0xFFu);
    buffer[4] = self.type;
    buffer[5] = self.q;
    buffer[6] = self.width_blocks;
    buffer[7] = self.height_blocks;
}

void SmolRTSP_JpegQtHeader_serialize(
    SmolRTSP_JpegQtHeader self, uint8_t buffer[restrict]) {
    /* RFC 2435 §3.1.8 wire layout:
     *   byte 0     -- MBZ
     *   byte 1     -- Precision
     *   bytes 2..3 -- Length (16-bit, big-endian) */
    buffer[0] = self.mbz;
    buffer[1] = self.precision;
    buffer[2] = (uint8_t)((self.length >> 8) & 0xFFu);
    buffer[3] = (uint8_t)(self.length & 0xFFu);
}
