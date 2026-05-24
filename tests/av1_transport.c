#include <smolrtsp/av1.h>
#include <smolrtsp/av1_transport.h>
#include <smolrtsp/rtp_transport.h>
#include <smolrtsp/transport.h>

#include <greatest.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* Wire layout of one RTP packet emitted by SmolRTSP_RtpTransport with
 * CSRC count 0 and no extension: 12-byte RTP header + N-byte payload.
 * For AV1 the first payload byte is the AOMedia AV1 RTP §4.4
 * aggregation header. */
#define RTP_HEADER_LEN 12
#define MIN_PACKET_LEN (RTP_HEADER_LEN + SMOLRTSP_AV1_AGGREGATION_HEADER_SIZE)
#define RX_BUF_SIZE    4096

typedef struct {
    bool marker;
    uint8_t payload_ty;
    uint16_t seq;
    uint32_t timestamp;
    /* Aggregation header bits. */
    bool z;
    bool y;
    uint8_t w;
    bool n;
    /* Everything after the aggregation byte. */
    size_t body_len;
    uint8_t body[RX_BUF_SIZE];
} ParsedPacket;

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

    const uint8_t agg = buf[RTP_HEADER_LEN];
    out->z = (agg & 0x80) != 0;
    out->y = (agg & 0x40) != 0;
    out->w = (uint8_t)((agg >> 4) & 0x03);
    out->n = (agg & 0x08) != 0;

    out->body_len = (size_t)n - MIN_PACKET_LEN;
    memcpy(out->body, buf + MIN_PACKET_LEN, out->body_len);
    return 0;
}

typedef struct {
    int tx;
    int rx;
    SmolRTSP_Av1Transport *av1;
} Fixture;

static Fixture make_fixture(SmolRTSP_Av1TransportConfig cfg, uint8_t pt) {
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) != 0) {
        abort();
    }

    srand(1);
    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);
    SmolRTSP_RtpTransport *rtp =
        SmolRTSP_RtpTransport_new(udp, pt, /*clock_rate=*/90000);
    SmolRTSP_Av1Transport *av1 =
        SmolRTSP_Av1Transport_new_with_config(rtp, cfg);

    return (Fixture){.tx = fds[0], .rx = fds[1], .av1 = av1};
}

static void drop_fixture(Fixture *f) {
    VTABLE(SmolRTSP_Av1Transport, SmolRTSP_Droppable).drop(f->av1);
    close(f->rx);
}

/* --- Helpers to build canonical OBU streams ------------------------- */

/* Build a one-byte OBU header with the given type and has_size_field. */
static uint8_t make_obu_header(uint8_t obu_type, bool has_size) {
    /* bit 0 forbidden=0, bits 1-4 obu_type, bit 5 ext=0,
     * bit 6 has_size, bit 7 reserved=0. */
    return (uint8_t)(((obu_type & 0x0Fu) << 3) | (has_size ? 0x02u : 0u));
}

/* --- Tests ---------------------------------------------------------- */

TEST tu_with_td_seqhdr_and_frame(void) {
    Fixture f = make_fixture(SmolRTSP_Av1TransportConfig_default(), 96);

    /* TU = TD + Sequence Header + Frame.
     * TD's payload is empty; Seq Hdr carries 5 bytes; Frame carries 10. */
    uint8_t seq_payload[5];
    uint8_t frame_payload[10];
    for (size_t i = 0; i < sizeof seq_payload; i++)
        seq_payload[i] = 0xA0u + i;
    for (size_t i = 0; i < sizeof frame_payload; i++)
        frame_payload[i] = 0xB0u + i;

    uint8_t tu[64];
    size_t p = 0;
    /* TD: type=2, has_size=1, size=0. */
    tu[p++] = make_obu_header(SMOLRTSP_AV1_OBU_TEMPORAL_DELIMITER, true);
    tu[p++] = 0x00; /* obu_size LEB128 = 0 */
    /* Seq header: type=1, has_size=1, size=5. */
    tu[p++] = make_obu_header(SMOLRTSP_AV1_OBU_SEQUENCE_HEADER, true);
    tu[p++] = (uint8_t)sizeof seq_payload;
    memcpy(tu + p, seq_payload, sizeof seq_payload);
    p += sizeof seq_payload;
    /* Frame: type=6, has_size=1, size=10. */
    tu[p++] = make_obu_header(SMOLRTSP_AV1_OBU_FRAME, true);
    tu[p++] = (uint8_t)sizeof frame_payload;
    memcpy(tu + p, frame_payload, sizeof frame_payload);
    p += sizeof frame_payload;

    ASSERT_EQ(
        0, SmolRTSP_Av1Transport_send_temporal_unit(
               f.av1, SmolRTSP_RtpTimestamp_Raw(9000), U8Slice99_new(tu, p),
               /*is_new_cvs=*/true));

    /* Packet 0: aggregation N=1 W=1 + seq header element. */
    ParsedPacket pkt;
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT_EQ_FMT((uint8_t)96, pkt.payload_ty, "%u");
    ASSERT_EQ_FMT((uint32_t)9000, pkt.timestamp, "%u");
    ASSERT(!pkt.z);
    ASSERT(!pkt.y);
    ASSERT_EQ_FMT((uint8_t)1, pkt.w, "%u");
    ASSERT(pkt.n);
    ASSERT(!pkt.marker);

    /* Body = (cleared seq header byte) + 5 payload bytes. */
    ASSERT_EQ_FMT((size_t)6, pkt.body_len, "%zu");
    ASSERT_EQ(
        (uint8_t)0x08, pkt.body[0]); /* seq header with size bit cleared */
    ASSERT_MEM_EQ(seq_payload, pkt.body + 1, sizeof seq_payload);

    /* Packet 1: aggregation W=1 N=0 + frame element, M=1. */
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT(!pkt.z);
    ASSERT(!pkt.y);
    ASSERT_EQ_FMT((uint8_t)1, pkt.w, "%u");
    ASSERT(!pkt.n);
    ASSERT(pkt.marker);
    ASSERT_EQ_FMT((size_t)11, pkt.body_len, "%zu");
    ASSERT_EQ(
        (uint8_t)0x30, pkt.body[0]); /* frame header with size bit cleared */
    ASSERT_MEM_EQ(frame_payload, pkt.body + 1, sizeof frame_payload);

    drop_fixture(&f);
    PASS();
}

TEST tu_only_td_emits_nothing(void) {
    Fixture f = make_fixture(SmolRTSP_Av1TransportConfig_default(), 96);

    uint8_t tu[2];
    tu[0] = make_obu_header(SMOLRTSP_AV1_OBU_TEMPORAL_DELIMITER, true);
    tu[1] = 0x00; /* size 0 */

    ASSERT_EQ(
        0, SmolRTSP_Av1Transport_send_temporal_unit(
               f.av1, SmolRTSP_RtpTimestamp_Raw(123),
               U8Slice99_new(tu, sizeof tu), /*is_new_cvs=*/true));

    /* Receiver should see nothing; close fixture and trust the underlying
     * datagram boundary (any unread packets would have to be drained).
     * If something was sent, the next read would return data; use a
     * non-blocking read to check. */
    int flags = recv(f.rx, NULL, 0, MSG_PEEK | MSG_DONTWAIT);
    ASSERT_EQ(-1, flags); /* nothing to peek */

    drop_fixture(&f);
    PASS();
}

TEST tu_tile_list_dropped(void) {
    Fixture f = make_fixture(SmolRTSP_Av1TransportConfig_default(), 96);

    /* Tile list (type 8) + Frame (type 6). The tile list must be dropped
     * before packetization, leaving exactly one packet. */
    uint8_t tu[16];
    size_t p = 0;
    tu[p++] = make_obu_header(SMOLRTSP_AV1_OBU_TILE_LIST, true);
    tu[p++] = 0x03;
    tu[p++] = 0xAA;
    tu[p++] = 0xAA;
    tu[p++] = 0xAA;
    tu[p++] = make_obu_header(SMOLRTSP_AV1_OBU_FRAME, true);
    tu[p++] = 0x02;
    tu[p++] = 0xCC;
    tu[p++] = 0xDD;

    ASSERT_EQ(
        0, SmolRTSP_Av1Transport_send_temporal_unit(
               f.av1, SmolRTSP_RtpTimestamp_Raw(456), U8Slice99_new(tu, p),
               /*is_new_cvs=*/false));

    ParsedPacket pkt;
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT(pkt.marker);
    ASSERT(!pkt.n);
    ASSERT_EQ_FMT((size_t)3, pkt.body_len, "%zu");
    ASSERT_EQ((uint8_t)0x30, pkt.body[0]);
    ASSERT_EQ((uint8_t)0xCC, pkt.body[1]);
    ASSERT_EQ((uint8_t)0xDD, pkt.body[2]);

    /* No second packet. */
    int flags = recv(f.rx, NULL, 0, MSG_PEEK | MSG_DONTWAIT);
    ASSERT_EQ(-1, flags);

    drop_fixture(&f);
    PASS();
}

TEST tu_large_obu_fragments_with_zy(void) {
    /* Force a tiny MTU so a single OBU spans multiple packets and
     * exercises the Z/Y continuation flags. With max_packet_size = 20:
     *   per_packet_budget = 20 - 1 = 19 bytes of element data per packet
     *   element = 1 (header) + 50 (payload) = 51 bytes
     *   chunks = 19, 19, 13. Three packets. */
    SmolRTSP_Av1TransportConfig cfg = SmolRTSP_Av1TransportConfig_default();
    cfg.max_packet_size = 20;
    Fixture f = make_fixture(cfg, 96);

    uint8_t payload[50];
    for (size_t i = 0; i < sizeof payload; i++)
        payload[i] = (uint8_t)i;

    uint8_t tu[64];
    size_t p = 0;
    tu[p++] = make_obu_header(SMOLRTSP_AV1_OBU_FRAME, true);
    tu[p++] = (uint8_t)sizeof payload;
    memcpy(tu + p, payload, sizeof payload);
    p += sizeof payload;

    ASSERT_EQ(
        0, SmolRTSP_Av1Transport_send_temporal_unit(
               f.av1, SmolRTSP_RtpTimestamp_Raw(7777), U8Slice99_new(tu, p),
               /*is_new_cvs=*/true));

    ParsedPacket pkt;

    /* Packet 0: Z=0, Y=1, N=1, W=1. Body = 1 prefix + 18 payload. */
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT(!pkt.z);
    ASSERT(pkt.y);
    ASSERT(pkt.n);
    ASSERT_EQ_FMT((uint8_t)1, pkt.w, "%u");
    ASSERT(!pkt.marker);
    ASSERT_EQ_FMT((size_t)19, pkt.body_len, "%zu");
    ASSERT_EQ((uint8_t)0x30, pkt.body[0]);
    ASSERT_MEM_EQ(payload, pkt.body + 1, 18);

    /* Packet 1: Z=1, Y=1, N=0. Body = 19 payload bytes (offsets 18..36). */
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT(pkt.z);
    ASSERT(pkt.y);
    ASSERT(!pkt.n);
    ASSERT(!pkt.marker);
    ASSERT_EQ_FMT((size_t)19, pkt.body_len, "%zu");
    ASSERT_MEM_EQ(payload + 18, pkt.body, 19);

    /* Packet 2: Z=1, Y=0, N=0, marker=1. Body = 13 payload bytes
     * (offsets 37..49). */
    ASSERT_EQ(0, recv_packet(f.rx, &pkt));
    ASSERT(pkt.z);
    ASSERT(!pkt.y);
    ASSERT(!pkt.n);
    ASSERT(pkt.marker);
    ASSERT_EQ_FMT((size_t)13, pkt.body_len, "%zu");
    ASSERT_MEM_EQ(payload + 37, pkt.body, 13);

    drop_fixture(&f);
    PASS();
}

TEST tu_two_obus_marker_only_on_last(void) {
    /* Two non-fragmenting OBUs in a TU. Verify M is set only on the
     * last packet, N only on the first when is_new_cvs is true. */
    Fixture f = make_fixture(SmolRTSP_Av1TransportConfig_default(), 96);

    uint8_t tu[16];
    size_t p = 0;
    tu[p++] = make_obu_header(SMOLRTSP_AV1_OBU_FRAME_HEADER, true);
    tu[p++] = 0x01;
    tu[p++] = 0x11;
    tu[p++] = make_obu_header(SMOLRTSP_AV1_OBU_TILE_GROUP, true);
    tu[p++] = 0x02;
    tu[p++] = 0x22;
    tu[p++] = 0x33;

    ASSERT_EQ(
        0, SmolRTSP_Av1Transport_send_temporal_unit(
               f.av1, SmolRTSP_RtpTimestamp_Raw(50), U8Slice99_new(tu, p),
               /*is_new_cvs=*/false));

    ParsedPacket p0, p1;
    ASSERT_EQ(0, recv_packet(f.rx, &p0));
    ASSERT_EQ(0, recv_packet(f.rx, &p1));

    ASSERT(!p0.n);
    ASSERT(!p0.marker);
    ASSERT(!p1.n);
    ASSERT(p1.marker);
    /* Seq increments by 1. */
    ASSERT_EQ_FMT((uint16_t)(p0.seq + 1), p1.seq, "%u");

    drop_fixture(&f);
    PASS();
}

SUITE(av1_transport) {
    RUN_TEST(tu_with_td_seqhdr_and_frame);
    RUN_TEST(tu_only_td_emits_nothing);
    RUN_TEST(tu_tile_list_dropped);
    RUN_TEST(tu_large_obu_fragments_with_zy);
    RUN_TEST(tu_two_obus_marker_only_on_last);
}
