#include <smolrtsp/types/rtcp.h>

#include <assert.h>
#include <string.h>

#include <arpa/inet.h>

#include <slice99.h>

#define RTCP_HEADER_VERSION_SHIFT 6
#define RTCP_HEADER_PADDING_SHIFT 5
#define RTCP_VERSION              2

/* Pack the common 4-byte RTCP packet header into the first 4 bytes of
 * `buffer`. `length_words` is the value of the wire length field (total
 * packet size in 32-bit words minus one). */
static uint8_t *write_rtcp_common_header(
    uint8_t buffer[restrict], bool padding, uint8_t sc_or_rc, uint8_t pt,
    uint16_t length_words) {
    *buffer = ((uint8_t)RTCP_VERSION << RTCP_HEADER_VERSION_SHIFT) |
              (sc_or_rc & 0x1f);
    if (padding) {
        *buffer |= ((uint8_t)1 << RTCP_HEADER_PADDING_SHIFT);
    }
    buffer++;
    *buffer = pt;
    buffer++;
    const uint16_t length_be = htons(length_words);
    buffer = SLICE99_APPEND(buffer, length_be);
    return buffer;
}

size_t SmolRTSP_RtcpSr_size(SmolRTSP_RtcpSr self) {
    (void)self;
    return SMOLRTSP_RTCP_SR_SIZE_BASE;
}

uint8_t *
SmolRTSP_RtcpSr_serialize(SmolRTSP_RtcpSr self, uint8_t buffer[restrict]) {
    assert(buffer);
    assert(self.rc == 0 && "RTCP SR reception report blocks not yet supported");

    uint8_t *buffer_backup = buffer;

    /* Length in 32-bit words minus one. For a base SR (28 B = 7 words),
     * length = 6. */
    buffer = write_rtcp_common_header(
        buffer, self.padding, self.rc, (uint8_t)SMOLRTSP_RTCP_PT_SR,
        (uint16_t)((SMOLRTSP_RTCP_SR_SIZE_BASE / 4u) - 1u));

    buffer = SLICE99_APPEND(buffer, self.ssrc);
    buffer = SLICE99_APPEND(buffer, self.ntp_sec);
    buffer = SLICE99_APPEND(buffer, self.ntp_frac);
    buffer = SLICE99_APPEND(buffer, self.rtp_ts);
    buffer = SLICE99_APPEND(buffer, self.pkt_count);
    buffer = SLICE99_APPEND(buffer, self.octet_count);

    return buffer_backup;
}

size_t SmolRTSP_RtcpRr_size(SmolRTSP_RtcpRr self) {
    (void)self;
    return SMOLRTSP_RTCP_RR_SIZE_BASE;
}

uint8_t *
SmolRTSP_RtcpRr_serialize(SmolRTSP_RtcpRr self, uint8_t buffer[restrict]) {
    assert(buffer);
    assert(self.rc == 0 && "RTCP RR reception report blocks not yet supported");

    uint8_t *buffer_backup = buffer;

    /* Length in 32-bit words minus one. Base RR is 8 B = 2 words, so
     * length = 1. */
    buffer = write_rtcp_common_header(
        buffer, self.padding, self.rc, (uint8_t)SMOLRTSP_RTCP_PT_RR,
        (uint16_t)((SMOLRTSP_RTCP_RR_SIZE_BASE / 4u) - 1u));

    buffer = SLICE99_APPEND(buffer, self.ssrc);
    return buffer_backup;
}

/* SDES with one chunk + one CNAME item + 1-byte type-0 terminator,
 * zero-padded so the total packet length is a multiple of 4. */
size_t SmolRTSP_RtcpSdesCname_size(SmolRTSP_RtcpSdesCname self) {
    assert(self.cname);
    const size_t cname_len = strlen(self.cname);
    assert(cname_len <= 255);
    /* 4 header + 4 ssrc + 2 (CNAME type+len) + cname_len text + 1 terminator */
    const size_t raw = 4 + 4 + 2 + cname_len + 1;
    return (raw + 3u) & ~(size_t)3u;
}

uint8_t *SmolRTSP_RtcpSdesCname_serialize(
    SmolRTSP_RtcpSdesCname self, uint8_t buffer[restrict]) {
    assert(buffer);
    assert(self.cname);

    const size_t cname_len = strlen(self.cname);
    assert(cname_len <= 255);
    const size_t total = SmolRTSP_RtcpSdesCname_size(self);
    uint8_t *const start = buffer;

    /* SC = 1 (one chunk). length = total/4 - 1. */
    buffer = write_rtcp_common_header(
        buffer, self.padding, /*sc_or_rc=*/1u, (uint8_t)SMOLRTSP_RTCP_PT_SDES,
        (uint16_t)((total / 4u) - 1u));

    buffer = SLICE99_APPEND(buffer, self.ssrc);

    /* CNAME item: type + length + text. */
    *buffer++ = (uint8_t)SMOLRTSP_RTCP_SDES_CNAME;
    *buffer++ = (uint8_t)cname_len;
    memcpy(buffer, self.cname, cname_len);
    buffer += cname_len;

    /* Item-list terminator (type 0) then zero-pad to 32-bit boundary. */
    *buffer++ = 0;
    while ((size_t)(buffer - start) < total) {
        *buffer++ = 0;
    }

    return start;
}

size_t SmolRTSP_RtcpBye_size(SmolRTSP_RtcpBye self) {
    /* 4 header + 4 ssrc. Plus, if reason present:
     *   1 byte length + N reason bytes, padded to 32-bit boundary. */
    if (self.reason == NULL || self.reason[0] == '\0') {
        return 8;
    }
    const size_t r_len = strlen(self.reason);
    assert(r_len <= 255);
    const size_t raw = 4 + 4 + 1 + r_len;
    return (raw + 3u) & ~(size_t)3u;
}

uint8_t *
SmolRTSP_RtcpBye_serialize(SmolRTSP_RtcpBye self, uint8_t buffer[restrict]) {
    assert(buffer);

    const size_t total = SmolRTSP_RtcpBye_size(self);
    uint8_t *const start = buffer;

    /* SC = 1 (one SSRC). */
    buffer = write_rtcp_common_header(
        buffer, self.padding, /*sc_or_rc=*/1u, (uint8_t)SMOLRTSP_RTCP_PT_BYE,
        (uint16_t)((total / 4u) - 1u));

    buffer = SLICE99_APPEND(buffer, self.ssrc);

    if (self.reason != NULL && self.reason[0] != '\0') {
        const size_t r_len = strlen(self.reason);
        assert(r_len <= 255);
        *buffer++ = (uint8_t)r_len;
        memcpy(buffer, self.reason, r_len);
        buffer += r_len;
        while ((size_t)(buffer - start) < total) {
            *buffer++ = 0;
        }
    }

    return start;
}
