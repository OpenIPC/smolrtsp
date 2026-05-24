#include <smolrtsp/jpegxs_transport.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <slice99.h>

static int send_unit(
    SmolRTSP_JpegXsTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegXsInterlace interlace, uint16_t initial_sep, bool is_frame_end,
    U8Slice99 unit);

SmolRTSP_JpegXsTransportConfig SmolRTSP_JpegXsTransportConfig_default(void) {
    return (SmolRTSP_JpegXsTransportConfig){
        .pack_mode = SMOLRTSP_JPEGXS_PACK_CODESTREAM,
        .sequential = true,
        .max_packet_size = SMOLRTSP_MAX_JPEGXS_PACKET_SIZE,
    };
}

struct SmolRTSP_JpegXsTransport {
    SmolRTSP_RtpTransport *transport;
    SmolRTSP_JpegXsTransportConfig config;
    uint8_t frame_counter; /* F (mod 32) */
};

SmolRTSP_JpegXsTransport *
SmolRTSP_JpegXsTransport_new(SmolRTSP_RtpTransport *t) {
    return SmolRTSP_JpegXsTransport_new_with_config(
        t, SmolRTSP_JpegXsTransportConfig_default());
}

SmolRTSP_JpegXsTransport *SmolRTSP_JpegXsTransport_new_with_config(
    SmolRTSP_RtpTransport *t, SmolRTSP_JpegXsTransportConfig config) {
    assert(t);
    assert(config.max_packet_size > SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE);
    /* RFC 9134 §4.3: "If T=0, slice packetization mode SHALL be used and
     * K SHALL be set to 1." */
    assert(config.sequential || config.pack_mode == SMOLRTSP_JPEGXS_PACK_SLICE);

    SmolRTSP_JpegXsTransport *self = malloc(sizeof *self);
    assert(self);

    self->transport = t;
    self->config = config;
    self->frame_counter = 0;

    return self;
}

static void SmolRTSP_JpegXsTransport_drop(VSelf) {
    VSELF(SmolRTSP_JpegXsTransport);
    assert(self);

    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(self->transport);

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_JpegXsTransport);

bool SmolRTSP_JpegXsTransport_is_full(SmolRTSP_JpegXsTransport *self) {
    return SmolRTSP_RtpTransport_is_full(self->transport);
}

int SmolRTSP_JpegXsTransport_send_codestream(
    SmolRTSP_JpegXsTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegXsInterlace interlace, bool is_frame_end,
    U8Slice99 codestream) {
    assert(self);
    assert(self->config.pack_mode == SMOLRTSP_JPEGXS_PACK_CODESTREAM);

    return send_unit(self, ts, interlace, 0, is_frame_end, codestream);
}

int SmolRTSP_JpegXsTransport_send_header_segment(
    SmolRTSP_JpegXsTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegXsInterlace interlace, U8Slice99 header_segment) {
    assert(self);
    assert(self->config.pack_mode == SMOLRTSP_JPEGXS_PACK_SLICE);

    /* The header segment is its own packetization unit and never carries
     * the frame-end marker. */
    return send_unit(
        self, ts, interlace, SMOLRTSP_JPEGXS_SEP_HEADER_SEGMENT, false,
        header_segment);
}

int SmolRTSP_JpegXsTransport_send_slice(
    SmolRTSP_JpegXsTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegXsInterlace interlace, uint16_t slice_index, bool is_frame_end,
    U8Slice99 slice) {
    assert(self);
    assert(self->config.pack_mode == SMOLRTSP_JPEGXS_PACK_SLICE);
    assert(slice_index < SMOLRTSP_JPEGXS_SEP_HEADER_SEGMENT);

    return send_unit(self, ts, interlace, slice_index, is_frame_end, slice);
}

/* Send one packetization unit. Chunks @p unit into RTP packets of at most
 * `config.max_packet_size` bytes (including the 4-byte JPEG XS payload
 * header), builds the per-packet header, sets L on the final chunk, and
 * sets the RTP marker bit when @p is_frame_end is true and on that same
 * final chunk. Bumps the internal frame counter (mod 32) after a
 * successful send with @p is_frame_end true. */
static int send_unit(
    SmolRTSP_JpegXsTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegXsInterlace interlace, uint16_t initial_sep, bool is_frame_end,
    U8Slice99 unit) {
    const size_t chunk_budget =
        self->config.max_packet_size - SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE;
    /* ceil(unit.len / chunk_budget), but always at least one packet so
     * that a zero-length unit still carries the L bit and (potentially)
     * the marker. */
    const size_t total_chunks =
        0 == unit.len ? 1 : (unit.len + chunk_budget - 1) / chunk_budget;

    uint16_t sep = initial_sep;
    uint16_t p = 0;
    size_t offset = 0;

    for (size_t i = 0; i < total_chunks; i++) {
        const size_t remaining = unit.len - offset;
        const size_t this_chunk =
            remaining < chunk_budget ? remaining : chunk_budget;
        const bool is_last_chunk = (i == total_chunks - 1);

        const SmolRTSP_JpegXsPayloadHeader pl_hdr = {
            .sequential = self->config.sequential,
            .pack_mode = self->config.pack_mode,
            .last_in_unit = is_last_chunk,
            .interlace = interlace,
            .frame_counter = self->frame_counter,
            .sep_counter = sep,
            .packet_counter = p,
        };

        uint8_t hdr_buf[SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE];
        SmolRTSP_JpegXsPayloadHeader_serialize(pl_hdr, hdr_buf);

        const U8Slice99 chunk = U8Slice99_new(unit.ptr + offset, this_chunk);
        const bool marker = is_frame_end && is_last_chunk;

        if (SmolRTSP_RtpTransport_send_packet(
                self->transport, ts, marker,
                U8Slice99_new(hdr_buf, sizeof hdr_buf), chunk) == -1) {
            return -1;
        }

        offset += this_chunk;

        if (self->config.pack_mode == SMOLRTSP_JPEGXS_PACK_CODESTREAM) {
            /* RFC 9134 §4.3 codestream mode: SEP starts at 0 for a new
             * packetization unit and is incremented each time P wraps
             * from 0x7FF back to 0. */
            if (0x7FF == p) {
                p = 0;
                sep = (uint16_t)((sep + 1) & 0x7FF);
            } else {
                p++;
            }
        } else {
            /* Slice mode: SEP encodes the slice index (or sentinel for
             * the header segment) for the entire unit. P just wraps. */
            p = (uint16_t)((p + 1) & 0x7FF);
        }
    }

    if (is_frame_end) {
        self->frame_counter = (uint8_t)((self->frame_counter + 1) & 0x1F);
    }

    return 0;
}
