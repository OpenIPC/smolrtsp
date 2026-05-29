#include <smolrtsp/jpeg.h>

#include <greatest.h>

TEST main_header_serialize_all_zero(void) {
    const SmolRTSP_JpegPayloadHeader h = {
        .type_specific = 0,
        .fragment_offset = 0,
        .type = 0,
        .q = 0,
        .width_blocks = 0,
        .height_blocks = 0,
    };

    uint8_t buf[SMOLRTSP_JPEG_MAIN_HEADER_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF,
                                                   0xFF, 0xFF, 0xFF, 0xFF};
    SmolRTSP_JpegPayloadHeader_serialize(h, buf);

    for (size_t i = 0; i < sizeof buf; i++) {
        ASSERT_EQ_FMT((uint8_t)0x00, buf[i], "%u");
    }
    PASS();
}

TEST main_header_serialize_field_layout(void) {
    /* Hand-computed sentinel:
     *   byte 0       type_specific   = 0x11
     *   bytes 1..3   fragment_offset = 0x123456 (24-bit big-endian)
     *   byte 4       type            = 0x42
     *   byte 5       q               = 0xFF (dynamic table)
     *   byte 6       width_blocks    = 0xF0 (= 1920 / 8 << 0 etc.)
     *   byte 7       height_blocks   = 0x88 (= 1088 / 8 etc.) */
    const SmolRTSP_JpegPayloadHeader h = {
        .type_specific = 0x11,
        .fragment_offset = 0x00123456u,
        .type = 0x42,
        .q = 0xFF,
        .width_blocks = 0xF0,
        .height_blocks = 0x88,
    };

    uint8_t buf[SMOLRTSP_JPEG_MAIN_HEADER_SIZE] = {0};
    SmolRTSP_JpegPayloadHeader_serialize(h, buf);

    ASSERT_EQ_FMT((uint8_t)0x11, buf[0], "%u");
    ASSERT_EQ_FMT((uint8_t)0x12, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)0x34, buf[2], "%u");
    ASSERT_EQ_FMT((uint8_t)0x56, buf[3], "%u");
    ASSERT_EQ_FMT((uint8_t)0x42, buf[4], "%u");
    ASSERT_EQ_FMT((uint8_t)0xFF, buf[5], "%u");
    ASSERT_EQ_FMT((uint8_t)0xF0, buf[6], "%u");
    ASSERT_EQ_FMT((uint8_t)0x88, buf[7], "%u");
    PASS();
}

TEST main_header_fragment_offset_truncates_to_24_bits(void) {
    /* The on-wire field is 24 bits; the top byte of a 32-bit
     * fragment_offset is silently dropped. */
    const SmolRTSP_JpegPayloadHeader h = {
        .type_specific = 0,
        .fragment_offset = 0xFFABCDEFu, /* top 0xFF must vanish */
        .type = 0,
        .q = 0,
        .width_blocks = 0,
        .height_blocks = 0,
    };

    uint8_t buf[SMOLRTSP_JPEG_MAIN_HEADER_SIZE] = {0};
    SmolRTSP_JpegPayloadHeader_serialize(h, buf);

    ASSERT_EQ_FMT((uint8_t)0xAB, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)0xCD, buf[2], "%u");
    ASSERT_EQ_FMT((uint8_t)0xEF, buf[3], "%u");
    PASS();
}

TEST qt_header_serialize_two_baseline_tables(void) {
    /* The classic case: two 8-bit baseline tables means precision=0,
     * length=128. Total wire bytes: 4. */
    const SmolRTSP_JpegQtHeader h = {
        .mbz = 0,
        .precision = 0,
        .length = 128,
    };

    uint8_t buf[SMOLRTSP_JPEG_QT_HEADER_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF};
    SmolRTSP_JpegQtHeader_serialize(h, buf);

    ASSERT_EQ_FMT((uint8_t)0x00, buf[0], "%u");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)0x00, buf[2], "%u"); /* length high byte */
    ASSERT_EQ_FMT((uint8_t)0x80, buf[3], "%u"); /* length low byte = 128 */
    PASS();
}

TEST qt_header_serialize_precision_bitmap(void) {
    /* Spot-check that the precision byte is written verbatim and that
     * Length is big-endian. */
    const SmolRTSP_JpegQtHeader h = {
        .mbz = 0,
        .precision = 0x03, /* tables 0 and 1 are both 16-bit */
        .length = 0x0140,  /* two 128-byte (16-bit) tables */
    };

    uint8_t buf[SMOLRTSP_JPEG_QT_HEADER_SIZE] = {0};
    SmolRTSP_JpegQtHeader_serialize(h, buf);

    ASSERT_EQ_FMT((uint8_t)0x00, buf[0], "%u");
    ASSERT_EQ_FMT((uint8_t)0x03, buf[1], "%u");
    ASSERT_EQ_FMT((uint8_t)0x01, buf[2], "%u");
    ASSERT_EQ_FMT((uint8_t)0x40, buf[3], "%u");
    PASS();
}

SUITE(jpeg) {
    RUN_TEST(main_header_serialize_all_zero);
    RUN_TEST(main_header_serialize_field_layout);
    RUN_TEST(main_header_fragment_offset_truncates_to_24_bits);
    RUN_TEST(qt_header_serialize_two_baseline_tables);
    RUN_TEST(qt_header_serialize_precision_bitmap);
}
