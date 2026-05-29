#include <smolrtsp/jpeg.h>
#include <smolrtsp/jpeg_transport.h>
#include <smolrtsp/rtp_transport.h>
#include <smolrtsp/transport.h>

#include <greatest.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* Wire layout of one RTP packet emitted by SmolRTSP_RtpTransport with
 * CSRC count 0 and no extension: 12-byte RTP header + N-byte payload.
 * For JPEG the first 8 payload bytes are the RFC 2435 main header;
 * the first packet of a frame may additionally carry a 4-byte QT
 * header plus the table bytes. */
#define RTP_HEADER_LEN     12
#define JPEG_MAIN_LEN      SMOLRTSP_JPEG_MAIN_HEADER_SIZE
#define JPEG_QT_HEADER_LEN SMOLRTSP_JPEG_QT_HEADER_SIZE
#define MIN_PACKET_LEN     (RTP_HEADER_LEN + JPEG_MAIN_LEN)
#define RX_BUF_SIZE        4096

typedef struct {
    bool marker;
    uint8_t payload_ty;
    uint16_t seq;
    uint32_t timestamp;
    uint32_t ssrc;
    /* RFC 2435 main header, parsed. */
    uint8_t ts_field;
    uint32_t fragment_offset;
    uint8_t type;
    uint8_t q;
    uint8_t width_blocks;
    uint8_t height_blocks;
    /* QT block (only meaningful on the first packet of a frame). */
    bool qt_block_present;
    uint8_t qt_mbz;
    uint8_t qt_precision;
    uint16_t qt_length;
    uint8_t qt_data[2 * SMOLRTSP_JPEG_QT_SIZE];
    /* Scan-data chunk carried by this packet. */
    size_t chunk_len;
    uint8_t chunk[RX_BUF_SIZE];
} ParsedPacket;

/* Read one packet from the receive side of a SOCK_SEQPACKET pair and
 * decode the RTP base header plus the JPEG payload.
 *
 * The `expect_qt_block` flag tells us whether to parse a QT block off
 * the front of the payload (true on the first packet of a Q >= 128
 * frame; false otherwise). The wire itself does not self-describe
 * this -- a real receiver derives it from the Q value plus a per-RTP-
 * session cache. */
static int recv_packet(int fd, bool expect_qt_block, ParsedPacket *out) {
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

    const uint8_t *jpg = buf + RTP_HEADER_LEN;
    out->ts_field = jpg[0];
    out->fragment_offset =
        ((uint32_t)jpg[1] << 16) | ((uint32_t)jpg[2] << 8) | jpg[3];
    out->type = jpg[4];
    out->q = jpg[5];
    out->width_blocks = jpg[6];
    out->height_blocks = jpg[7];

    size_t cursor = MIN_PACKET_LEN;
    out->qt_block_present = false;
    if (expect_qt_block) {
        if ((ssize_t)(cursor + JPEG_QT_HEADER_LEN) > n) {
            return -1;
        }
        const uint8_t *qt = buf + cursor;
        out->qt_block_present = true;
        out->qt_mbz = qt[0];
        out->qt_precision = qt[1];
        out->qt_length = ((uint16_t)qt[2] << 8) | qt[3];
        cursor += JPEG_QT_HEADER_LEN;
        if ((ssize_t)(cursor + out->qt_length) > n ||
            out->qt_length > sizeof out->qt_data) {
            return -1;
        }
        memcpy(out->qt_data, buf + cursor, out->qt_length);
        cursor += out->qt_length;
    }

    out->chunk_len = (size_t)n - cursor;
    memcpy(out->chunk, buf + cursor, out->chunk_len);
    return 0;
}

typedef struct {
    int tx;
    int rx;
    SmolRTSP_JpegTransport *jpg;
} Fixture;

static Fixture make_fixture(SmolRTSP_JpegTransportConfig cfg, uint8_t pt) {
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) != 0) {
        abort();
    }

    srand(1);
    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);
    SmolRTSP_RtpTransport *rtp =
        SmolRTSP_RtpTransport_new(udp, pt, /*clock_rate=*/90000);
    SmolRTSP_JpegTransport *jpg =
        SmolRTSP_JpegTransport_new_with_config(rtp, cfg);

    return (Fixture){.tx = fds[0], .rx = fds[1], .jpg = jpg};
}

static void drop_fixture(Fixture *f) {
    VTABLE(SmolRTSP_JpegTransport, SmolRTSP_Droppable).drop(f->jpg);
    close(f->rx);
}

/* Build a deterministic 64-byte quantization table. The byte values
 * themselves are arbitrary -- the test only cares that they survive
 * the round trip into and back out of the wire. */
static void fill_qt(uint8_t *qt, uint8_t seed) {
    for (size_t i = 0; i < SMOLRTSP_JPEG_QT_SIZE; i++) {
        qt[i] = (uint8_t)(seed + (uint8_t)i);
    }
}

TEST single_packet_frame_with_qt(void) {
    Fixture f = make_fixture(SmolRTSP_JpegTransportConfig_default(), /*pt=*/26);

    uint8_t qt0[SMOLRTSP_JPEG_QT_SIZE];
    uint8_t qt1[SMOLRTSP_JPEG_QT_SIZE];
    fill_qt(qt0, 0x10);
    fill_qt(qt1, 0x80);
    uint8_t scan[200];
    for (size_t i = 0; i < sizeof scan; i++) {
        scan[i] = (uint8_t)(0xC0 + (i & 0x3F));
    }

    const SmolRTSP_JpegFrame frame = {
        .hdr =
            {.type_specific = 0,
             .fragment_offset = 0,
             .type = 1,
             .q = 0xFF,
             .width_blocks = 240, /* 1920 / 8 */
             .height_blocks = 135 /* 1080 / 8 */},
        .qt0 = U8Slice99_new(qt0, sizeof qt0),
        .qt1 = U8Slice99_new(qt1, sizeof qt1),
        .scan_data = U8Slice99_new(scan, sizeof scan),
    };

    ASSERT_EQ(
        0, SmolRTSP_JpegTransport_send_frame(
               f.jpg, SmolRTSP_RtpTimestamp_Raw(9000), frame));

    ParsedPacket pkt;
    ASSERT_EQ(0, recv_packet(f.rx, /*expect_qt_block=*/true, &pkt));

    ASSERT(pkt.marker);
    ASSERT_EQ_FMT((uint8_t)26, pkt.payload_ty, "%u");
    ASSERT_EQ_FMT((uint32_t)9000, pkt.timestamp, "%u");
    ASSERT_EQ_FMT((uint32_t)0, pkt.fragment_offset, "%u");
    ASSERT_EQ_FMT((uint8_t)1, pkt.type, "%u");
    ASSERT_EQ_FMT((uint8_t)0xFF, pkt.q, "%u");
    ASSERT_EQ_FMT((uint8_t)240, pkt.width_blocks, "%u");
    ASSERT_EQ_FMT((uint8_t)135, pkt.height_blocks, "%u");

    ASSERT(pkt.qt_block_present);
    ASSERT_EQ_FMT((uint8_t)0, pkt.qt_mbz, "%u");
    ASSERT_EQ_FMT((uint8_t)0, pkt.qt_precision, "%u");
    ASSERT_EQ_FMT((uint16_t)128, pkt.qt_length, "%u");
    ASSERT_MEM_EQ(qt0, pkt.qt_data, sizeof qt0);
    ASSERT_MEM_EQ(qt1, pkt.qt_data + sizeof qt0, sizeof qt1);

    ASSERT_EQ_FMT((size_t)sizeof scan, pkt.chunk_len, "%zu");
    ASSERT_MEM_EQ(scan, pkt.chunk, sizeof scan);

    drop_fixture(&f);
    PASS();
}

TEST fragmented_frame_with_qt(void) {
    /* Tight MTU so a small scan-data buffer spans several packets and
     * the first-packet header overhead is visible.
     *
     *   max_packet_size = 220
     *   first packet  : 8 main + 4 qt-hdr + 64 qt0 + 64 qt1 = 140 hdr,
     *                   so 80 bytes of scan data fit.
     *   cont packets  : 8 main, so 212 bytes of scan data fit.
     *   400 byte scan : 80 + 212 + 108 = 400. Expect 3 packets. */
    SmolRTSP_JpegTransportConfig cfg = SmolRTSP_JpegTransportConfig_default();
    cfg.max_packet_size = 220;
    Fixture f = make_fixture(cfg, /*pt=*/26);

    uint8_t qt0[SMOLRTSP_JPEG_QT_SIZE];
    uint8_t qt1[SMOLRTSP_JPEG_QT_SIZE];
    fill_qt(qt0, 0x20);
    fill_qt(qt1, 0x90);
    uint8_t scan[400];
    for (size_t i = 0; i < sizeof scan; i++) {
        scan[i] = (uint8_t)(i & 0xFF);
    }

    const SmolRTSP_JpegFrame frame = {
        .hdr =
            {.type_specific = 0,
             .fragment_offset = 0,
             .type = 1,
             .q = 0xFE,
             .width_blocks = 80,
             .height_blocks = 60},
        .qt0 = U8Slice99_new(qt0, sizeof qt0),
        .qt1 = U8Slice99_new(qt1, sizeof qt1),
        .scan_data = U8Slice99_new(scan, sizeof scan),
    };

    ASSERT_EQ(
        0, SmolRTSP_JpegTransport_send_frame(
               f.jpg, SmolRTSP_RtpTimestamp_Raw(1234), frame));

    const size_t expected_chunks[] = {80, 212, 108};
    size_t total = 0;
    for (size_t i = 0; i < 3; i++) {
        ParsedPacket pkt;
        const bool first = (i == 0);
        ASSERT_EQ(0, recv_packet(f.rx, /*expect_qt_block=*/first, &pkt));

        const bool last = (i == 2);
        ASSERT_EQ(last, pkt.marker);
        ASSERT_EQ_FMT((uint32_t)1234, pkt.timestamp, "%u");
        ASSERT_EQ_FMT((uint32_t)total, pkt.fragment_offset, "%u");
        /* The per-frame fields are stamped on every packet. */
        ASSERT_EQ_FMT((uint8_t)1, pkt.type, "%u");
        ASSERT_EQ_FMT((uint8_t)0xFE, pkt.q, "%u");
        ASSERT_EQ_FMT((uint8_t)80, pkt.width_blocks, "%u");
        ASSERT_EQ_FMT((uint8_t)60, pkt.height_blocks, "%u");

        ASSERT_EQ(first, pkt.qt_block_present);
        if (first) {
            ASSERT_EQ_FMT((uint16_t)128, pkt.qt_length, "%u");
            ASSERT_MEM_EQ(qt0, pkt.qt_data, sizeof qt0);
            ASSERT_MEM_EQ(qt1, pkt.qt_data + sizeof qt0, sizeof qt1);
        }

        ASSERT_EQ_FMT(expected_chunks[i], pkt.chunk_len, "%zu");
        ASSERT_MEM_EQ(scan + total, pkt.chunk, pkt.chunk_len);
        total += pkt.chunk_len;
    }
    ASSERT_EQ_FMT((size_t)sizeof scan, total, "%zu");

    drop_fixture(&f);
    PASS();
}

TEST referenced_tables_no_qt_block(void) {
    /* Q < 128 case: the caller passes empty quantization-table slices,
     * so no QT block goes on the wire and the first-packet header is
     * just the 8-byte main header. */
    Fixture f = make_fixture(SmolRTSP_JpegTransportConfig_default(), /*pt=*/26);

    uint8_t scan[100];
    memset(scan, 0xA5, sizeof scan);

    const SmolRTSP_JpegFrame frame = {
        .hdr =
            {.type_specific = 0,
             .fragment_offset = 0,
             .type = 0,
             .q = 50, /* < SMOLRTSP_JPEG_Q_DYNAMIC */
             .width_blocks = 40,
             .height_blocks = 30},
        .qt0 = U8Slice99_empty(),
        .qt1 = U8Slice99_empty(),
        .scan_data = U8Slice99_new(scan, sizeof scan),
    };

    ASSERT_EQ(
        0, SmolRTSP_JpegTransport_send_frame(
               f.jpg, SmolRTSP_RtpTimestamp_Raw(42), frame));

    ParsedPacket pkt;
    ASSERT_EQ(0, recv_packet(f.rx, /*expect_qt_block=*/false, &pkt));

    ASSERT(pkt.marker);
    ASSERT_EQ_FMT((uint8_t)50, pkt.q, "%u");
    ASSERT_EQ_FMT((uint32_t)0, pkt.fragment_offset, "%u");
    ASSERT(!pkt.qt_block_present);
    ASSERT_EQ_FMT((size_t)sizeof scan, pkt.chunk_len, "%zu");
    ASSERT_MEM_EQ(scan, pkt.chunk, sizeof scan);

    drop_fixture(&f);
    PASS();
}

TEST seq_advances_across_packets_and_frames(void) {
    /* Sanity-check that SmolRTSP_RtpTransport keeps the sequence number
     * and SSRC consistent across both intra-frame fragments and frame
     * boundaries. */
    SmolRTSP_JpegTransportConfig cfg = SmolRTSP_JpegTransportConfig_default();
    cfg.max_packet_size = 60; /* tiny: forces fragmentation */
    Fixture f = make_fixture(cfg, /*pt=*/26);

    uint8_t scan[120];
    memset(scan, 0x33, sizeof scan);
    const SmolRTSP_JpegFrame frame = {
        .hdr =
            {.type_specific = 0,
             .fragment_offset = 0,
             .type = 0,
             .q = 60,
             .width_blocks = 10,
             .height_blocks = 10},
        .qt0 = U8Slice99_empty(),
        .qt1 = U8Slice99_empty(),
        .scan_data = U8Slice99_new(scan, sizeof scan),
    };

    /* First frame: 8 + 52 -byte cont packets / 60 MTU =>
     * first 52 bytes, next 52, last 16 = 3 packets. */
    ASSERT_EQ(
        0, SmolRTSP_JpegTransport_send_frame(
               f.jpg, SmolRTSP_RtpTimestamp_Raw(1000), frame));

    ParsedPacket pkts[6];
    for (size_t i = 0; i < 3; i++) {
        ASSERT_EQ(0, recv_packet(f.rx, /*expect_qt_block=*/false, &pkts[i]));
    }
    /* Second frame: same shape, 3 more packets. */
    ASSERT_EQ(
        0, SmolRTSP_JpegTransport_send_frame(
               f.jpg, SmolRTSP_RtpTimestamp_Raw(2000), frame));
    for (size_t i = 3; i < 6; i++) {
        ASSERT_EQ(0, recv_packet(f.rx, /*expect_qt_block=*/false, &pkts[i]));
    }

    for (size_t i = 1; i < 6; i++) {
        ASSERT_EQ_FMT(pkts[0].ssrc, pkts[i].ssrc, "%u");
        ASSERT_EQ_FMT((uint16_t)(pkts[0].seq + (uint16_t)i), pkts[i].seq, "%u");
    }
    /* Marker on the last packet of each frame only. */
    ASSERT(!pkts[0].marker);
    ASSERT(!pkts[1].marker);
    ASSERT(pkts[2].marker);
    ASSERT(!pkts[3].marker);
    ASSERT(!pkts[4].marker);
    ASSERT(pkts[5].marker);
    /* The same timestamp shared by all packets of one frame. */
    ASSERT_EQ_FMT((uint32_t)1000, pkts[0].timestamp, "%u");
    ASSERT_EQ_FMT((uint32_t)1000, pkts[2].timestamp, "%u");
    ASSERT_EQ_FMT((uint32_t)2000, pkts[3].timestamp, "%u");
    ASSERT_EQ_FMT((uint32_t)2000, pkts[5].timestamp, "%u");

    drop_fixture(&f);
    PASS();
}

SUITE(jpeg_transport) {
    RUN_TEST(single_packet_frame_with_qt);
    RUN_TEST(fragmented_frame_with_qt);
    RUN_TEST(referenced_tables_no_qt_block);
    RUN_TEST(seq_advances_across_packets_and_frames);
}
