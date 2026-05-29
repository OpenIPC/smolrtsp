#include <smolrtsp/jpeg_transport.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <slice99.h>

/* RFC 2435 caps Width and Height (in 8-pixel units) at 8 bits each, but
 * Fragment Offset is 24-bit -- so the largest representable scan data is
 * 0xFFFFFF bytes. A larger frame is a caller bug. */
#define SMOLRTSP_PRIV_JPEG_MAX_FRAGMENT_OFFSET 0xFFFFFFu

SmolRTSP_JpegTransportConfig SmolRTSP_JpegTransportConfig_default(void) {
    return (SmolRTSP_JpegTransportConfig){
        .max_packet_size = SMOLRTSP_MAX_JPEG_PACKET_SIZE,
    };
}

struct SmolRTSP_JpegTransport {
    SmolRTSP_RtpTransport *transport;
    SmolRTSP_JpegTransportConfig config;
};

SmolRTSP_JpegTransport *SmolRTSP_JpegTransport_new(SmolRTSP_RtpTransport *t) {
    return SmolRTSP_JpegTransport_new_with_config(
        t, SmolRTSP_JpegTransportConfig_default());
}

SmolRTSP_JpegTransport *SmolRTSP_JpegTransport_new_with_config(
    SmolRTSP_RtpTransport *t, SmolRTSP_JpegTransportConfig config) {
    assert(t);
    assert(config.max_packet_size > SMOLRTSP_JPEG_MAIN_HEADER_SIZE);

    SmolRTSP_JpegTransport *self = malloc(sizeof *self);
    assert(self);

    self->transport = t;
    self->config = config;

    return self;
}

static void SmolRTSP_JpegTransport_drop(VSelf) {
    VSELF(SmolRTSP_JpegTransport);
    assert(self);

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(self->transport);

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_JpegTransport);

bool SmolRTSP_JpegTransport_is_full(SmolRTSP_JpegTransport *self) {
    return SmolRTSP_RtpTransport_is_full(self->transport);
}

/* Layout of the per-packet header buffer:
 *
 *   first packet, with embedded QT (qt_block_size > 0):
 *     [ 8 B main JPEG header ][ 4 B QT header ][ qt0.len ][ qt1.len ][ scan ]
 *
 *   first packet, no embedded QT (qt_block_size == 0):
 *     [ 8 B main JPEG header ][ scan ]
 *
 *   continuation packets:
 *     [ 8 B main JPEG header ][ scan ]
 *
 * The longest header we ever emit is 8 + 4 + 64 + 64 = 140 bytes (two
 * 8-bit baseline tables). Anything larger means qt0/qt1 are non-baseline
 * tables; the assertion in send_frame catches sizes that wouldn't leave
 * room for a single scan-data byte. */
#define SMOLRTSP_PRIV_JPEG_HDR_BUF                                             \
    (SMOLRTSP_JPEG_MAIN_HEADER_SIZE + SMOLRTSP_JPEG_QT_HEADER_SIZE +           \
     2 * SMOLRTSP_JPEG_QT_SIZE)

int SmolRTSP_JpegTransport_send_frame(
    SmolRTSP_JpegTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegFrame frame) {
    assert(self);
    assert(frame.qt0.len + frame.qt1.len <= 0xFFFFu);
    assert(frame.scan_data.len <= SMOLRTSP_PRIV_JPEG_MAX_FRAGMENT_OFFSET);

    const size_t qt_block_size =
        (0 == frame.qt0.len && 0 == frame.qt1.len)
            ? 0
            : SMOLRTSP_JPEG_QT_HEADER_SIZE + frame.qt0.len + frame.qt1.len;
    const size_t first_hdr_size =
        SMOLRTSP_JPEG_MAIN_HEADER_SIZE + qt_block_size;
    const size_t cont_hdr_size = SMOLRTSP_JPEG_MAIN_HEADER_SIZE;

    /* Reject configurations that wouldn't leave room for even one scan-data
     * byte on the first packet -- a single-byte first packet is
     * representable but useless. The check is loose: callers can still
     * tune max_packet_size so that continuation packets carry strictly
     * more scan data than the first one. */
    assert(self->config.max_packet_size > first_hdr_size);
    assert(first_hdr_size <= SMOLRTSP_PRIV_JPEG_HDR_BUF);

    const size_t first_chunk_budget =
        self->config.max_packet_size - first_hdr_size;
    const size_t cont_chunk_budget =
        self->config.max_packet_size - cont_hdr_size;

    uint8_t hdr_buf[SMOLRTSP_PRIV_JPEG_HDR_BUF];

    size_t offset = 0;
    bool first = true;
    do {
        const size_t budget = first ? first_chunk_budget : cont_chunk_budget;
        const size_t remaining = frame.scan_data.len - offset;
        const size_t this_chunk = remaining < budget ? remaining : budget;
        const bool is_last = (offset + this_chunk == frame.scan_data.len);

        /* Stamp this packet's fragment offset; everything else in the
         * main header is fixed for the frame. */
        SmolRTSP_JpegPayloadHeader pl_hdr = frame.hdr;
        pl_hdr.fragment_offset = (uint32_t)offset;

        size_t hdr_len = SMOLRTSP_JPEG_MAIN_HEADER_SIZE;
        SmolRTSP_JpegPayloadHeader_serialize(pl_hdr, hdr_buf);

        if (first && qt_block_size > 0) {
            const SmolRTSP_JpegQtHeader qt_hdr = {
                .mbz = 0,
                .precision = 0, /* baseline: all tables are 8-bit */
                .length = (uint16_t)(frame.qt0.len + frame.qt1.len),
            };
            SmolRTSP_JpegQtHeader_serialize(qt_hdr, hdr_buf + hdr_len);
            hdr_len += SMOLRTSP_JPEG_QT_HEADER_SIZE;
            if (frame.qt0.len > 0) {
                memcpy(hdr_buf + hdr_len, frame.qt0.ptr, frame.qt0.len);
                hdr_len += frame.qt0.len;
            }
            if (frame.qt1.len > 0) {
                memcpy(hdr_buf + hdr_len, frame.qt1.ptr, frame.qt1.len);
                hdr_len += frame.qt1.len;
            }
        }

        const U8Slice99 chunk =
            (0 == frame.scan_data.len)
                ? U8Slice99_empty()
                : U8Slice99_new(frame.scan_data.ptr + offset, this_chunk);

        if (SmolRTSP_RtpTransport_send_packet(
                self->transport, ts, /*marker=*/is_last,
                U8Slice99_new(hdr_buf, hdr_len), chunk) == -1) {
            return -1;
        }

        offset += this_chunk;
        first = false;
    } while (offset < frame.scan_data.len);

    return 0;
}
