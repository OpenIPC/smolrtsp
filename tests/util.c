#include <smolrtsp/util.h>

#include <inttypes.h>
#include <stdint.h>

#include <greatest.h>

TEST parse_transport_config(void) {
    SmolRTSP_TransportConfig config;
    memset(&config, '\0', sizeof config);

    int ret = smolrtsp_parse_transport(
        &config,
        CharSlice99_from_str(
            "RTP/AVP/UDP;unicast;client_port=3056-3057;interleaved=4-5"));
    ASSERT_EQ(0, ret);
    ASSERT_EQ(SmolRTSP_LowerTransport_UDP, config.lower);
    ASSERT(config.unicast);
    ASSERT(!config.multicast);

    match(config.interleaved) {
        of(SmolRTSP_ChannelPair_Some, val) {
            ASSERT_EQ(4, val->rtp_channel);
            ASSERT_EQ(5, val->rtcp_channel);
        }
        otherwise FAIL();
    }

    match(config.client_port) {
        of(SmolRTSP_PortPair_Some, val) {
            ASSERT_EQ(3056, val->rtp_port);
            ASSERT_EQ(3057, val->rtcp_port);
        }
        otherwise FAIL();
    }

    PASS();
}

TEST parse_transport_minimal(void) {
    SmolRTSP_TransportConfig config;
    memset(&config, '\0', sizeof config);

#define CHECK_REST                                                             \
    do {                                                                       \
        ASSERT(!config.unicast);                                               \
        ASSERT(!config.multicast);                                             \
        ASSERT(MATCHES(config.interleaved, SmolRTSP_ChannelPair_None));        \
        ASSERT(MATCHES(config.client_port, SmolRTSP_PortPair_None));           \
    } while (0)

    int ret =
        smolrtsp_parse_transport(&config, CharSlice99_from_str("RTP/AVP"));
    ASSERT_EQ(0, ret);
    ASSERT_EQ(SmolRTSP_LowerTransport_UDP, config.lower);
    CHECK_REST;

    ret =
        smolrtsp_parse_transport(&config, CharSlice99_from_str("RTP/AVP/TCP"));
    ASSERT_EQ(0, ret);
    ASSERT_EQ(SmolRTSP_LowerTransport_TCP, config.lower);
    CHECK_REST;

    ret =
        smolrtsp_parse_transport(&config, CharSlice99_from_str("RTP/AVP/UDP"));
    ASSERT_EQ(0, ret);
    ASSERT_EQ(SmolRTSP_LowerTransport_UDP, config.lower);
    CHECK_REST;

#undef CHECK_REST

    PASS();
}

TEST parse_transport_trailing_semicolon(void) {
    SmolRTSP_TransportConfig config;
    memset(&config, '\0', sizeof config);

    int ret = smolrtsp_parse_transport(
        &config,
        CharSlice99_from_str(
            "RTP/AVP/UDP;unicast;client_port=3056-3057;interleaved=4-5;"));
    ASSERT_EQ(0, ret);
    ASSERT_EQ(SmolRTSP_LowerTransport_UDP, config.lower);
    ASSERT(config.unicast);
    ASSERT(!config.multicast);

    match(config.interleaved) {
        of(SmolRTSP_ChannelPair_Some, val) {
            ASSERT_EQ(4, val->rtp_channel);
            ASSERT_EQ(5, val->rtcp_channel);
        }
        otherwise FAIL();
    }

    match(config.client_port) {
        of(SmolRTSP_PortPair_Some, val) {
            ASSERT_EQ(3056, val->rtp_port);
            ASSERT_EQ(3057, val->rtcp_port);
        }
        otherwise FAIL();
    }

    PASS();
}

TEST interleaved_header(void) {
    uint8_t channel_id = 123;
    uint16_t payload_len = 54321;

    const uint32_t binary =
        smolrtsp_interleaved_header(channel_id, payload_len);

    ASSERT_EQ(0xd4317b24, binary);

    PASS();
}

TEST parse_interleaved_header(void) {
    const uint8_t data[] = {'$', 42, 0b00110000, 0b00111001};
    uint8_t channel_id = 0;
    uint16_t payload_len = 0;

    smolrtsp_parse_interleaved_header(data, &channel_id, &payload_len);

    ASSERT_EQ_FMT(42, channel_id, "%" PRIu8);
    ASSERT_EQ_FMT(12345, payload_len, "%" PRIu16);

    PASS();
}

TEST parse_interleaved_frame_complete(void) {
    /* $, channel=7, len=3 (big-endian 0x0003), payload "abc", trailing "x" */
    const uint8_t data[] = {'$', 7, 0x00, 0x03, 'a', 'b', 'c', 'x'};
    CharSlice99 input = CharSlice99_new((char *)(uintptr_t)data, sizeof data);

    uint8_t channel_id = 0;
    U8Slice99 payload = U8Slice99_empty();
    size_t consumed = 0;

    const SmolRTSP_InterleavedFrameStatus status =
        smolrtsp_parse_interleaved_frame(
            input, &channel_id, &payload, &consumed);

    ASSERT_EQ(SmolRTSP_InterleavedFrameStatus_Complete, status);
    ASSERT_EQ_FMT(7, channel_id, "%" PRIu8);
    ASSERT_EQ_FMT((size_t)3, payload.len, "%zu");
    ASSERT_EQ('a', (char)payload.ptr[0]);
    ASSERT_EQ('b', (char)payload.ptr[1]);
    ASSERT_EQ('c', (char)payload.ptr[2]);
    ASSERT_EQ_FMT((size_t)7, consumed, "%zu"); /* 4-byte header + 3 payload */

    PASS();
}

TEST parse_interleaved_frame_partial_header(void) {
    /* Only the $ byte arrived so far. */
    const uint8_t data[] = {'$'};
    CharSlice99 input = CharSlice99_new((char *)(uintptr_t)data, sizeof data);

    uint8_t channel_id = 99;
    U8Slice99 payload = U8Slice99_empty();
    size_t consumed = 999;

    const SmolRTSP_InterleavedFrameStatus status =
        smolrtsp_parse_interleaved_frame(
            input, &channel_id, &payload, &consumed);

    ASSERT_EQ(SmolRTSP_InterleavedFrameStatus_Partial, status);
    /* Outputs untouched on Partial. */
    ASSERT_EQ_FMT(99, channel_id, "%" PRIu8);
    ASSERT_EQ_FMT((size_t)999, consumed, "%zu");

    PASS();
}

TEST parse_interleaved_frame_partial_payload(void) {
    /* Header says len=10 but only 2 payload bytes have arrived. */
    const uint8_t data[] = {'$', 3, 0x00, 0x0a, 'x', 'y'};
    CharSlice99 input = CharSlice99_new((char *)(uintptr_t)data, sizeof data);

    uint8_t channel_id = 0;
    U8Slice99 payload = U8Slice99_empty();
    size_t consumed = 0;

    const SmolRTSP_InterleavedFrameStatus status =
        smolrtsp_parse_interleaved_frame(
            input, &channel_id, &payload, &consumed);

    ASSERT_EQ(SmolRTSP_InterleavedFrameStatus_Partial, status);

    PASS();
}

TEST parse_interleaved_frame_not_interleaved(void) {
    /* Buffer starts with an RTSP request, not a $-frame. */
    const char request[] = "OPTIONS rtsp://x RTSP/1.0\r\n";
    CharSlice99 input = CharSlice99_from_str((char *)request);

    uint8_t channel_id = 0;
    U8Slice99 payload = U8Slice99_empty();
    size_t consumed = 0;

    const SmolRTSP_InterleavedFrameStatus status =
        smolrtsp_parse_interleaved_frame(
            input, &channel_id, &payload, &consumed);

    ASSERT_EQ(SmolRTSP_InterleavedFrameStatus_NotInterleaved, status);

    PASS();
}

TEST parse_interleaved_frame_zero_length(void) {
    /* Valid frame with zero-byte payload. */
    const uint8_t data[] = {'$', 5, 0x00, 0x00};
    CharSlice99 input = CharSlice99_new((char *)(uintptr_t)data, sizeof data);

    uint8_t channel_id = 0;
    U8Slice99 payload = U8Slice99_new((uint8_t *)"sentinel", 8);
    size_t consumed = 0;

    const SmolRTSP_InterleavedFrameStatus status =
        smolrtsp_parse_interleaved_frame(
            input, &channel_id, &payload, &consumed);

    ASSERT_EQ(SmolRTSP_InterleavedFrameStatus_Complete, status);
    ASSERT_EQ_FMT(5, channel_id, "%" PRIu8);
    ASSERT_EQ_FMT((size_t)0, payload.len, "%zu");
    ASSERT_EQ_FMT((size_t)4, consumed, "%zu");

    PASS();
}

SUITE(util) {
    RUN_TEST(parse_transport_config);
    RUN_TEST(parse_transport_minimal);
    RUN_TEST(parse_transport_trailing_semicolon);
    RUN_TEST(interleaved_header);
    RUN_TEST(parse_interleaved_header);
    RUN_TEST(parse_interleaved_frame_complete);
    RUN_TEST(parse_interleaved_frame_partial_header);
    RUN_TEST(parse_interleaved_frame_partial_payload);
    RUN_TEST(parse_interleaved_frame_not_interleaved);
    RUN_TEST(parse_interleaved_frame_zero_length);
}
