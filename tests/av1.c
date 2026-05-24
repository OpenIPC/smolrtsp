#include <smolrtsp/av1.h>

#include <greatest.h>

#include <inttypes.h>
#include <string.h>

/* --- LEB128 round-trip ---------------------------------------------- */

TEST leb128_zero(void) {
    uint8_t buf[SMOLRTSP_AV1_LEB128_MAX_SIZE] = {0xFF};
    ASSERT_EQ((size_t)1, smolrtsp_av1_leb128_encode(0, buf));
    ASSERT_EQ((uint8_t)0x00, buf[0]);
    ASSERT_EQ((size_t)1, smolrtsp_av1_leb128_size(0));

    uint64_t v = 0xDEADBEEF;
    ASSERT_EQ((size_t)1, smolrtsp_av1_leb128_decode(buf, 1, &v));
    ASSERT_EQ_FMT((uint64_t)0, v, "%" PRIu64);
    PASS();
}

TEST leb128_one_byte_boundary(void) {
    /* 127 fits in one byte; 128 needs two. */
    uint8_t buf[SMOLRTSP_AV1_LEB128_MAX_SIZE];
    ASSERT_EQ((size_t)1, smolrtsp_av1_leb128_encode(127, buf));
    ASSERT_EQ((uint8_t)0x7F, buf[0]);

    ASSERT_EQ((size_t)2, smolrtsp_av1_leb128_encode(128, buf));
    ASSERT_EQ((uint8_t)0x80, buf[0]);
    ASSERT_EQ((uint8_t)0x01, buf[1]);

    ASSERT_EQ((size_t)1, smolrtsp_av1_leb128_size(127));
    ASSERT_EQ((size_t)2, smolrtsp_av1_leb128_size(128));
    PASS();
}

TEST leb128_round_trip_many(void) {
    const uint64_t cases[] = {
        0,       1,         63,          127,
        128,     200,       16383,       16384,
        0xFFFFu, 0x100000u, 0xFFFFFFFFu, UINT64_C(0x123456789ABCDEF0),
    };
    for (size_t i = 0; i < sizeof cases / sizeof *cases; i++) {
        uint8_t buf[SMOLRTSP_AV1_LEB128_MAX_SIZE];
        const size_t enc = smolrtsp_av1_leb128_encode(cases[i], buf);
        ASSERT_EQ(smolrtsp_av1_leb128_size(cases[i]), enc);

        uint64_t out = 0;
        const size_t dec = smolrtsp_av1_leb128_decode(buf, enc, &out);
        ASSERT_EQ(enc, dec);
        ASSERT_EQ_FMT(cases[i], out, "%" PRIu64);
    }
    PASS();
}

TEST leb128_decode_truncated(void) {
    /* A continuation byte with no successor must fail. */
    const uint8_t buf[] = {0x80};
    uint64_t v = 0xFEED;
    ASSERT_EQ((size_t)0, smolrtsp_av1_leb128_decode(buf, sizeof buf, &v));
    PASS();
}

/* --- Aggregation header serialize ----------------------------------- */

TEST agg_header_zero(void) {
    SmolRTSP_Av1AggregationHeader h = {0};
    uint8_t b = 0xFFu;
    SmolRTSP_Av1AggregationHeader_serialize(h, &b);
    ASSERT_EQ((uint8_t)0x00, b);
    PASS();
}

TEST agg_header_each_flag(void) {
    {
        SmolRTSP_Av1AggregationHeader h = {.z = true};
        uint8_t b;
        SmolRTSP_Av1AggregationHeader_serialize(h, &b);
        ASSERT_EQ((uint8_t)0x80, b);
    }
    {
        SmolRTSP_Av1AggregationHeader h = {.y = true};
        uint8_t b;
        SmolRTSP_Av1AggregationHeader_serialize(h, &b);
        ASSERT_EQ((uint8_t)0x40, b);
    }
    {
        SmolRTSP_Av1AggregationHeader h = {.n = true};
        uint8_t b;
        SmolRTSP_Av1AggregationHeader_serialize(h, &b);
        ASSERT_EQ((uint8_t)0x08, b);
    }
    PASS();
}

TEST agg_header_w(void) {
    for (uint8_t w = 0; w <= 3; w++) {
        SmolRTSP_Av1AggregationHeader h = {.w = w};
        uint8_t b;
        SmolRTSP_Av1AggregationHeader_serialize(h, &b);
        ASSERT_EQ_FMT((uint8_t)(w << 4), b, "0x%02x");
    }
    PASS();
}

TEST agg_header_all_set(void) {
    /* Z=Y=N=1, W=3 -> 0x80 | 0x40 | 0x30 | 0x08 = 0xF8 */
    SmolRTSP_Av1AggregationHeader h = {.z = true, .y = true, .w = 3, .n = true};
    uint8_t b;
    SmolRTSP_Av1AggregationHeader_serialize(h, &b);
    ASSERT_EQ((uint8_t)0xF8, b);
    PASS();
}

/* --- OBU header byte accessors -------------------------------------- */

TEST obu_header_accessors(void) {
    /* Sequence Header (type 1), no extension, has_size_field=1.
     *   bit 0: forbidden = 0
     *   bits 1-4: obu_type = 0001
     *   bit 5: extension_flag = 0
     *   bit 6: has_size_field = 1
     *   bit 7: reserved = 0
     * Byte = 0_0001_0_1_0 = 0b00001010 = 0x0A. */
    const uint8_t b = 0x0Au;
    ASSERT_EQ(
        (uint8_t)SMOLRTSP_AV1_OBU_SEQUENCE_HEADER, smolrtsp_av1_obu_type(b));
    ASSERT(!smolrtsp_av1_obu_has_extension(b));
    ASSERT(smolrtsp_av1_obu_has_size_field(b));
    ASSERT_EQ((uint8_t)0x08, smolrtsp_av1_obu_clear_size_field(b));
    PASS();
}

TEST obu_header_frame_with_extension(void) {
    /* OBU_FRAME (type 6), extension=1, has_size_field=1.
     *   0_0110_1_1_0 = 0b00110110 = 0x36. */
    const uint8_t b = 0x36u;
    ASSERT_EQ((uint8_t)SMOLRTSP_AV1_OBU_FRAME, smolrtsp_av1_obu_type(b));
    ASSERT(smolrtsp_av1_obu_has_extension(b));
    ASSERT(smolrtsp_av1_obu_has_size_field(b));
    /* clear has_size_field -> 0x34 */
    ASSERT_EQ((uint8_t)0x34, smolrtsp_av1_obu_clear_size_field(b));
    PASS();
}

SUITE(av1) {
    RUN_TEST(leb128_zero);
    RUN_TEST(leb128_one_byte_boundary);
    RUN_TEST(leb128_round_trip_many);
    RUN_TEST(leb128_decode_truncated);

    RUN_TEST(agg_header_zero);
    RUN_TEST(agg_header_each_flag);
    RUN_TEST(agg_header_w);
    RUN_TEST(agg_header_all_set);

    RUN_TEST(obu_header_accessors);
    RUN_TEST(obu_header_frame_with_extension);
}
