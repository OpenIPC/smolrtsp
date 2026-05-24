#include <smolrtsp/jpegxs.h>

#include <greatest.h>

TEST header_serialize_all_zero(void) {
    const SmolRTSP_JpegXsPayloadHeader h = {
        .sequential = false,
        .pack_mode = SMOLRTSP_JPEGXS_PACK_CODESTREAM,
        .last_in_unit = false,
        .interlace = SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
        .frame_counter = 0,
        .sep_counter = 0,
        .packet_counter = 0,
    };

    uint8_t buf[SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF};
    SmolRTSP_JpegXsPayloadHeader_serialize(h, buf);

    ASSERT_EQ(0x00, buf[0]);
    ASSERT_EQ(0x00, buf[1]);
    ASSERT_EQ(0x00, buf[2]);
    ASSERT_EQ(0x00, buf[3]);
    PASS();
}

TEST header_serialize_all_max(void) {
    /* Every defined bit set: T=K=L=1, I=0b11 (FIELD_2), F=31, SEP=0x7FF,
     * P=0x7FF -- fills the whole 32-bit word. */
    const SmolRTSP_JpegXsPayloadHeader h = {
        .sequential = true,
        .pack_mode = SMOLRTSP_JPEGXS_PACK_SLICE,
        .last_in_unit = true,
        .interlace = SMOLRTSP_JPEGXS_INTERLACE_FIELD_2,
        .frame_counter = 0x1F,
        .sep_counter = 0x7FF,
        .packet_counter = 0x7FF,
    };

    uint8_t buf[SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE] = {0};
    SmolRTSP_JpegXsPayloadHeader_serialize(h, buf);

    ASSERT_EQ(0xFF, buf[0]);
    ASSERT_EQ(0xFF, buf[1]);
    ASSERT_EQ(0xFF, buf[2]);
    ASSERT_EQ(0xFF, buf[3]);
    PASS();
}

TEST header_serialize_t_only(void) {
    const SmolRTSP_JpegXsPayloadHeader h = {
        .sequential = true,
        .pack_mode = SMOLRTSP_JPEGXS_PACK_CODESTREAM,
        .last_in_unit = false,
        .interlace = SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
        .frame_counter = 0,
        .sep_counter = 0,
        .packet_counter = 0,
    };

    uint8_t buf[SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE] = {0};
    SmolRTSP_JpegXsPayloadHeader_serialize(h, buf);

    ASSERT_EQ(0x80, buf[0]); /* MSB only */
    ASSERT_EQ(0x00, buf[1]);
    ASSERT_EQ(0x00, buf[2]);
    ASSERT_EQ(0x00, buf[3]);
    PASS();
}

TEST header_serialize_k_only(void) {
    const SmolRTSP_JpegXsPayloadHeader h = {
        .sequential = false,
        .pack_mode = SMOLRTSP_JPEGXS_PACK_SLICE,
        .last_in_unit = false,
        .interlace = SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
        .frame_counter = 0,
        .sep_counter = 0,
        .packet_counter = 0,
    };

    uint8_t buf[SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE] = {0};
    SmolRTSP_JpegXsPayloadHeader_serialize(h, buf);

    ASSERT_EQ(0x40, buf[0]);
    ASSERT_EQ(0x00, buf[1]);
    ASSERT_EQ(0x00, buf[2]);
    ASSERT_EQ(0x00, buf[3]);
    PASS();
}

TEST header_serialize_l_only(void) {
    const SmolRTSP_JpegXsPayloadHeader h = {
        .sequential = false,
        .pack_mode = SMOLRTSP_JPEGXS_PACK_CODESTREAM,
        .last_in_unit = true,
        .interlace = SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
        .frame_counter = 0,
        .sep_counter = 0,
        .packet_counter = 0,
    };

    uint8_t buf[SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE] = {0};
    SmolRTSP_JpegXsPayloadHeader_serialize(h, buf);

    ASSERT_EQ(0x20, buf[0]);
    ASSERT_EQ(0x00, buf[1]);
    ASSERT_EQ(0x00, buf[2]);
    ASSERT_EQ(0x00, buf[3]);
    PASS();
}

TEST header_serialize_field_layout(void) {
    /* Spot-check a hand-computed example so any future tweak to the bit
     * layout breaks loudly. T=1 K=1 L=1 I=11 F=00001 SEP=0x123 P=0x045
     *
     *   word = (1<<31)|(1<<30)|(1<<29)|(3<<27)|(1<<22)|(0x123<<11)|0x045
     *        = 0xF8400000 | 0x00091800 | 0x00000045
     *        = 0xF8491845
     */
    const SmolRTSP_JpegXsPayloadHeader h = {
        .sequential = true,
        .pack_mode = SMOLRTSP_JPEGXS_PACK_SLICE,
        .last_in_unit = true,
        .interlace = SMOLRTSP_JPEGXS_INTERLACE_FIELD_2,
        .frame_counter = 1,
        .sep_counter = 0x123,
        .packet_counter = 0x045,
    };

    uint8_t buf[SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE] = {0};
    SmolRTSP_JpegXsPayloadHeader_serialize(h, buf);

    ASSERT_EQ(0xF8, buf[0]);
    ASSERT_EQ(0x49, buf[1]);
    ASSERT_EQ(0x18, buf[2]);
    ASSERT_EQ(0x45, buf[3]);
    PASS();
}

TEST header_serialize_header_segment_sentinel(void) {
    /* In slice mode the header segment uses SEP=0x7FF as a sentinel. */
    const SmolRTSP_JpegXsPayloadHeader h = {
        .sequential = true,
        .pack_mode = SMOLRTSP_JPEGXS_PACK_SLICE,
        .last_in_unit = true,
        .interlace = SMOLRTSP_JPEGXS_INTERLACE_PROGRESSIVE,
        .frame_counter = 0,
        .sep_counter = SMOLRTSP_JPEGXS_SEP_HEADER_SEGMENT,
        .packet_counter = 0,
    };

    uint8_t buf[SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE] = {0};
    SmolRTSP_JpegXsPayloadHeader_serialize(h, buf);

    /* T=1 K=1 L=1 I=00 F=0 SEP=11111111111 P=0
     *   = 0xE0000000 | (0x7FF << 11)
     *   = 0xE0000000 | 0x003FF800
     *   = 0xE03FF800 */
    ASSERT_EQ(0xE0, buf[0]);
    ASSERT_EQ(0x3F, buf[1]);
    ASSERT_EQ(0xF8, buf[2]);
    ASSERT_EQ(0x00, buf[3]);
    PASS();
}

SUITE(jpegxs) {
    RUN_TEST(header_serialize_all_zero);
    RUN_TEST(header_serialize_all_max);
    RUN_TEST(header_serialize_t_only);
    RUN_TEST(header_serialize_k_only);
    RUN_TEST(header_serialize_l_only);
    RUN_TEST(header_serialize_field_layout);
    RUN_TEST(header_serialize_header_segment_sentinel);
}
