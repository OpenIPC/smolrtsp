#include <smolrtsp/types/rtcp.h>

#include <assert.h>
#include <string.h>

#include <arpa/inet.h>

#include <slice99.h>

#define RTCP_HEADER_VERSION_SHIFT 6
#define RTCP_HEADER_PADDING_SHIFT 5
#define RTCP_VERSION              2

size_t SmolRTSP_RtcpSr_size(SmolRTSP_RtcpSr self) {
    (void)self;
    return SMOLRTSP_RTCP_SR_SIZE_BASE;
}

uint8_t *
SmolRTSP_RtcpSr_serialize(SmolRTSP_RtcpSr self, uint8_t buffer[restrict]) {
    assert(buffer);
    assert(self.rc == 0 && "RTCP SR reception report blocks not yet supported");

    uint8_t *buffer_backup = buffer;

    /*
     *  0                   1                   2                   3
     *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |V=2|P|    RC   |   PT=SR=200   |             length            |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     */

    /* Pack V (2 bits), P (1 bit), RC (5 bits) into byte 0. */
    *buffer =
        ((uint8_t)RTCP_VERSION << RTCP_HEADER_VERSION_SHIFT) | (self.rc & 0x1f);
    if (self.padding) {
        *buffer |= ((uint8_t)1 << RTCP_HEADER_PADDING_SHIFT);
    }
    buffer++;

    /* PT (8 bits). */
    *buffer = (uint8_t)SMOLRTSP_RTCP_PT_SR;
    buffer++;

    /* Length: in 32-bit words minus one. For a base SR (28 B = 7 words),
     * length = 6. Network byte order. */
    const uint16_t length_be =
        htons((uint16_t)((SMOLRTSP_RTCP_SR_SIZE_BASE / 4u) - 1u));
    buffer = SLICE99_APPEND(buffer, length_be);

    buffer = SLICE99_APPEND(buffer, self.ssrc);
    buffer = SLICE99_APPEND(buffer, self.ntp_sec);
    buffer = SLICE99_APPEND(buffer, self.ntp_frac);
    buffer = SLICE99_APPEND(buffer, self.rtp_ts);
    buffer = SLICE99_APPEND(buffer, self.pkt_count);
    buffer = SLICE99_APPEND(buffer, self.octet_count);

    return buffer_backup;
}
