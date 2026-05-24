#include <smolrtsp/jpegxs_transport.h>
#include <smolrtsp/rtp_transport.h>
#include <smolrtsp/transport.h>

#include <greatest.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* Wire layout of one RTP packet emitted by SmolRTSP_RtpTransport with
 * CSRC count 0 and no extension: 12-byte RTP header + N-byte payload.
 * For JPEG XS the first 4 payload bytes are the RFC 9134 §4.3 header. */
#define RTP_HEADER_LEN 12
#define JXS_HEADER_LEN SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE
#define MIN_PACKET_LEN (RTP_HEADER_LEN + JXS_HEADER_LEN)
#define RX_BUF_SIZE    4096

typedef struct {
    bool marker;
    uint8_t payload_ty;
    uint16_t seq;
    uint32_t timestamp;
    uint32_t ssrc;
    /* JPEG XS payload header, parsed. */
    bool t;
    bool k;
    bool l;
    uint8_t i;
    uint8_t f;
    uint16_t sep;
    uint16_t p;
    /* JPEG XS chunk bytes (after the 4-byte payload header). */
    size_t chunk_len;
    uint8_t chunk[RX_BUF_SIZE];
} ParsedPacket;

/* Read one packet from the receive side of a SOCK_SEQPACKET pair and
 * decode the RTP base header and JPEG XS payload header. */
static int recv_packet(int fd, ParsedPacket *out) {
    uint8_t buf[RX_BUF_SIZE];
    const ssize_t n = read(fd, buf, sizeof buf);
    if (n < (ssize_t)MIN_PACKET_LEN) {
        return -1;
    }

    out->marker = (buf[1] & 0x80) != 0;
    out->payload_ty = buf[1] & 0x7F;
    out->seq = ((uint16_t)buf[2] << 8) | buf[3];
    out->timestamp = ((uint32_t)buf[4] << 24) | ((uint32_t)buf[5] << 16) |
                     ((uint32_t)buf[6] << 8) | buf[7];
    out->ssrc = ((uint32_t)buf[8] << 24) | ((uint32_t)buf[9] << 16) |
                ((uint32_t)buf[10] << 8) | buf[11];

    const uint8_t *jxs = buf + RTP_HEADER_LEN;
    const uint32_t w = ((uint32_t)jxs[0] << 24) | ((uint32_t)jxs[1] << 16) |
                       ((uint32_t)jxs[2] << 8) | jxs[3];
    out->t = (w >> 31) & 0x1;
    out->k = (w >> 30) & 0x1;
    out->l = (w >> 29) & 0x1;
    out->i = (w >> 27) & 0x3;
    out->f = (w >> 22) & 0x1F;
    out->sep = (w >> 11) & 0x7FF;
    out->p = w & 0x7FF;

    out->chunk_len = (size_t)n - MIN_PACKET_LEN;
    memcpy(out->chunk, buf + MIN_PACKET_LEN, out->chunk_len);
    return 0;
}

typedef struct {
    int tx; /* fd handed to the transport (the send side). */
    int rx; /* the receive side we read from. */
    SmolRTSP_JpegXsTransport *jxs;
} Fixture;

static Fixture make_fixture(SmolRTSP_JpegXsTransportConfig cfg, uint8_t pt) {
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) != 0) {
        abort();
    }

    srand(1);
    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);
    SmolRTSP_RtpTransport *rtp =
        SmolRTSP_RtpTransport_new(udp, pt, /*clock_rate=*/90000);
    SmolRTSP_JpegXsTransport *jxs =
        SmolRTSP_JpegXsTransport_new_with_config(rtp, cfg);

    return (Fixture){.tx = fds[0], .rx = fds[1], .jxs = jxs};
}

static void drop_fixture(Fixture *f) {
    VTABLE(SmolRTSP_JpegXsTransport, SmolRTSP_Droppable).drop(f->jxs);
    close(f->rx);
}

TEST codestream_single_packet_frame(void) {
    Fixture f = make_fixture(SmolRTSP_JpegXsTransportConfig_default(), 96);

    uint8_t body[100];
    memset(body, 0xAB, sizeof body);

    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_codestream(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(5000),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               /*is_frame_end=*/true, U8Slice99_new(body, sizeof body)));

    ParsedPacket pkt;
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));

    ASSERT(pkt.marker);
    ASSERT_EQ_FMT((uint8_t)96, pkt.payload_ty, "%u");
    ASSERT_EQ_FMT((uint32_t)5000, pkt.timestamp, "%u");

    ASSERT(pkt.t);  /* sequential default */
    ASSERT(!pkt.k); /* codestream mode */
    ASSERT(pkt.l);  /* last in unit */
    ASSERT_EQ_FMT((uint8_t)0, pkt.i, "%u");
    ASSERT_EQ_FMT((uint8_t)0, pkt.f, "%u");
    ASSERT_EQ_FMT((uint16_t)0, pkt.sep, "%u");
    ASSERT_EQ_FMT((uint16_t)0, pkt.p, "%u");

    ASSERT_EQ_FMT((size_t)sizeof body, pkt.chunk_len, "%zu");
    ASSERT_MEM_EQ(body, pkt.chunk, sizeof body);

    /* A second frame post-increments F to 1. */
    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_codestream(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(8000),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               /*is_frame_end=*/true, U8Slice99_new(body, sizeof body)));
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT_EQ_FMT((uint8_t)1, pkt.f, "%u");

    drop_fixture(&f);
    PASS();
}

TEST codestream_multipacket_frame(void) {
    SmolRTSP_JpegXsTransportConfig cfg =
        SmolRTSP_JpegXsTransportConfig_default();
    /* Small MTU so a modest payload spans several packets. Chunk budget
     * after the 4-byte JXS header = 50 - 4 = 46 bytes. */
    cfg.max_packet_size = 50;
    Fixture f = make_fixture(cfg, 96);

    /* 150 bytes / 46-byte chunks => 4 packets (46, 46, 46, 12). */
    uint8_t body[150];
    for (size_t i = 0; i < sizeof body; i++) {
        body[i] = (uint8_t)(i & 0xFF);
    }

    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_codestream(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(1000),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               /*is_frame_end=*/true, U8Slice99_new(body, sizeof body)));

    size_t total = 0;
    for (size_t i = 0; i < 4; i++) {
        ParsedPacket pkt;
        ASSERT_EQ(0, recv_packet(f.rx, &pkt));
        const bool is_last = (i == 3);
        ASSERT_EQ_FMT((uint16_t)i, pkt.p, "%u");
        ASSERT_EQ_FMT((uint16_t)0, pkt.sep, "%u");
        ASSERT_EQ(is_last, pkt.l);
        ASSERT_EQ(is_last, pkt.marker);
        ASSERT_EQ_FMT((uint32_t)1000, pkt.timestamp, "%u");
        ASSERT_MEM_EQ(body + total, pkt.chunk, pkt.chunk_len);
        total += pkt.chunk_len;
    }
    ASSERT_EQ_FMT((size_t)sizeof body, total, "%zu");

    drop_fixture(&f);
    PASS();
}

TEST codestream_interlaced_two_fields(void) {
    Fixture f = make_fixture(SmolRTSP_JpegXsTransportConfig_default(), 96);

    uint8_t body[64];
    memset(body, 0xCD, sizeof body);

    /* FIELD_1: not the frame end, so M is not set and F stays at 0. */
    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_codestream(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(7000),
               SMOLRTSP_JPEGXS_INTERLACE_FIELD_1,
               /*is_frame_end=*/false, U8Slice99_new(body, sizeof body)));
    ParsedPacket pkt;
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT_EQ_FMT((uint8_t)SMOLRTSP_JPEGXS_INTERLACE_FIELD_1, pkt.i, "%u");
    ASSERT(pkt.l);       /* last in this unit */
    ASSERT(!pkt.marker); /* but not last in frame */
    ASSERT_EQ_FMT((uint8_t)0, pkt.f, "%u");

    /* FIELD_2: completes the frame; both fields share F, then F bumps. */
    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_codestream(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(7000),
               SMOLRTSP_JPEGXS_INTERLACE_FIELD_2,
               /*is_frame_end=*/true, U8Slice99_new(body, sizeof body)));
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT_EQ_FMT((uint8_t)SMOLRTSP_JPEGXS_INTERLACE_FIELD_2, pkt.i, "%u");
    ASSERT(pkt.l);
    ASSERT(pkt.marker);
    ASSERT_EQ_FMT((uint8_t)0, pkt.f, "%u");

    /* Next progressive frame should now see F=1. */
    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_codestream(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(10000),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               /*is_frame_end=*/true, U8Slice99_new(body, sizeof body)));
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT_EQ_FMT((uint8_t)1, pkt.f, "%u");

    drop_fixture(&f);
    PASS();
}

TEST slice_mode_header_segment(void) {
    SmolRTSP_JpegXsTransportConfig cfg =
        SmolRTSP_JpegXsTransportConfig_default();
    cfg.pack_mode = SMOLRTSP_JPEGXS_PACK_SLICE;
    Fixture f = make_fixture(cfg, 96);

    uint8_t header[80];
    memset(header, 0x11, sizeof header);

    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_header_segment(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(2222),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               U8Slice99_new(header, sizeof header)));
    ParsedPacket pkt;
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));

    ASSERT(pkt.k); /* slice mode */
    ASSERT(pkt.l);
    ASSERT(!pkt.marker); /* header segment never ends a frame */
    ASSERT_EQ_FMT((uint16_t)SMOLRTSP_JPEGXS_SEP_HEADER_SEGMENT, pkt.sep, "%u");
    ASSERT_EQ_FMT((uint16_t)0, pkt.p, "%u");

    drop_fixture(&f);
    PASS();
}

TEST slice_mode_slice_with_marker(void) {
    SmolRTSP_JpegXsTransportConfig cfg =
        SmolRTSP_JpegXsTransportConfig_default();
    cfg.pack_mode = SMOLRTSP_JPEGXS_PACK_SLICE;
    Fixture f = make_fixture(cfg, 96);

    uint8_t slice[40];
    memset(slice, 0x22, sizeof slice);

    /* Two slices in a frame: index 0 (not last), then index 1 with
     * is_frame_end so the marker fires. */
    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_slice(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(3333),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               /*slice_index=*/0, /*is_frame_end=*/false,
               U8Slice99_new(slice, sizeof slice)));
    ParsedPacket pkt;
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT_EQ_FMT((uint16_t)0, pkt.sep, "%u");
    ASSERT(pkt.l);
    ASSERT(!pkt.marker);
    ASSERT_EQ_FMT((uint8_t)0, pkt.f, "%u");

    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_slice(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(3333),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               /*slice_index=*/1, /*is_frame_end=*/true,
               U8Slice99_new(slice, sizeof slice)));
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT_EQ_FMT((uint16_t)1, pkt.sep, "%u");
    ASSERT(pkt.l);
    ASSERT(pkt.marker);
    ASSERT_EQ_FMT((uint8_t)0, pkt.f, "%u");

    drop_fixture(&f);
    PASS();
}

TEST counters_and_seq_advance(void) {
    Fixture f = make_fixture(SmolRTSP_JpegXsTransportConfig_default(), 96);

    /* Save the underlying RTP transport's view by reading the SSRC the
     * library generated -- it must be identical across the two packets. */
    uint8_t body[10];
    memset(body, 0x99, sizeof body);

    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_codestream(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(100),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               /*is_frame_end=*/true, U8Slice99_new(body, sizeof body)));
    ParsedPacket p1;
    ASSERT_EQ(0, recv_packet(f.rx, &p1));

    ASSERT_EQ(
        0, SmolRTSP_JpegXsTransport_send_codestream(
               f.jxs, SmolRTSP_RtpTimestamp_Raw(200),
               SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
               /*is_frame_end=*/true, U8Slice99_new(body, sizeof body)));
    ParsedPacket p2;
    ASSERT_EQ(0, recv_packet(f.rx, &p2));

    ASSERT_EQ_FMT(p1.ssrc, p2.ssrc, "%u");
    ASSERT_EQ_FMT((uint16_t)(p1.seq + 1), p2.seq, "%u");

    drop_fixture(&f);
    PASS();
}

SUITE(jpegxs_transport) {
    RUN_TEST(codestream_single_packet_frame);
    RUN_TEST(codestream_multipacket_frame);
    RUN_TEST(codestream_interlaced_two_fields);
    RUN_TEST(slice_mode_header_segment);
    RUN_TEST(slice_mode_slice_with_marker);
    RUN_TEST(counters_and_seq_advance);
}
