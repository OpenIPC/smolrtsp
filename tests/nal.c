#include <smolrtsp/nal.h>

#include <greatest.h>

#include <alloca.h>

static const SmolRTSP_H264NalHeader h264_header = {
    .forbidden_zero_bit = false,
    .ref_idc = 0b10,
    .unit_type = SMOLRTSP_H264_NAL_UNIT_SUBSET_SPS,
};

static const SmolRTSP_H265NalHeader h265_header = {
    .forbidden_zero_bit = false,
    .unit_type = SMOLRTSP_H265_NAL_UNIT_RASL_N,
    .nuh_layer_id = 0b110101,
    .nuh_temporal_id_plus1 = 0b101,
};

#define NAL_HEADER_TEST_GETTER(T, name, h264_value, h265_value)                \
    TEST header_##name##_h264(void) {                                          \
        const T result =                                                       \
            SmolRTSP_NalHeader_##name(SmolRTSP_NalHeader_H264(h264_header));   \
        ASSERT_EQ(h264_value, result);                                         \
        PASS();                                                                \
    }                                                                          \
                                                                               \
    TEST header_##name##_h265(void) {                                          \
        const T result =                                                       \
            SmolRTSP_NalHeader_##name(SmolRTSP_NalHeader_H265(h265_header));   \
        ASSERT_EQ(h265_value, result);                                         \
        PASS();                                                                \
    }

NAL_HEADER_TEST_GETTER(
    uint8_t, unit_type, h264_header.unit_type, h265_header.unit_type)
NAL_HEADER_TEST_GETTER(
    size_t, size, SMOLRTSP_H264_NAL_HEADER_SIZE, SMOLRTSP_H265_NAL_HEADER_SIZE)
NAL_HEADER_TEST_GETTER(
    size_t, fu_size, SMOLRTSP_H264_FU_HEADER_SIZE, SMOLRTSP_H265_FU_HEADER_SIZE)

#undef NAL_HEADER_TEST_GETTER

TEST header_serialize_h264(void) {
    uint8_t buffer[SMOLRTSP_H264_NAL_HEADER_SIZE] = {0};
    SmolRTSP_NalHeader_serialize(SmolRTSP_NalHeader_H264(h264_header), buffer);
    ASSERT_EQ(0b01001111, buffer[0]);

    PASS();
}

TEST header_serialize_h265(void) {
    uint8_t buffer[SMOLRTSP_H265_NAL_HEADER_SIZE] = {0};
    SmolRTSP_NalHeader_serialize(SmolRTSP_NalHeader_H265(h265_header), buffer);
    ASSERT_EQ(0b00010001, buffer[0]);
    ASSERT_EQ(0b10101101, buffer[1]);

    PASS();
}

TEST header_serialize_fu_h264(void) {
    uint8_t buffer[SMOLRTSP_H264_FU_HEADER_SIZE] = {0};
    const bool is_first_fragment = true, is_last_fragment = false;

    SmolRTSP_NalHeader_write_fu_header(
        SmolRTSP_NalHeader_H264(h264_header), buffer, is_first_fragment,
        is_last_fragment);

    ASSERT_EQ(0b01011100, buffer[0]);
    ASSERT_EQ(0b10001111, buffer[1]);

    PASS();
}

TEST header_serialize_fu_h265(void) {
    uint8_t buffer[SMOLRTSP_H265_FU_HEADER_SIZE] = {0};
    const bool is_first_fragment = true, is_last_fragment = false;

    SmolRTSP_NalHeader_write_fu_header(
        SmolRTSP_NalHeader_H265(h265_header), buffer, is_first_fragment,
        is_last_fragment);

    ASSERT_EQ(0b01100011, buffer[0]);
    ASSERT_EQ(0b10101101, buffer[1]);
    ASSERT_EQ(0b10001000, buffer[2]);

    PASS();
}

TEST determine_start_code(void) {
    ASSERT_EQ(NULL, smolrtsp_determine_start_code(U8Slice99_empty()));

#define CHECK(expected, ...)                                                   \
    ASSERT_EQ(                                                                 \
        expected,                                                              \
        smolrtsp_determine_start_code(                                         \
            (U8Slice99)Slice99_typed_from_array((uint8_t[]){__VA_ARGS__})))

    CHECK(NULL, 0x00);
    CHECK(NULL, 0x00, 0x00);
    CHECK(smolrtsp_test_start_code_3b, 0x00, 0x00, 0x01);
    CHECK(smolrtsp_test_start_code_3b, 0x00, 0x00, 0x01, 0xAB);
    CHECK(smolrtsp_test_start_code_3b, 0x00, 0x00, 0x01, 0xAB, 0x8C);

    CHECK(smolrtsp_test_start_code_4b, 0x00, 0x00, 0x00, 0x01);
    CHECK(smolrtsp_test_start_code_4b, 0x00, 0x00, 0x00, 0x01, 0xAB);
    CHECK(smolrtsp_test_start_code_4b, 0x00, 0x00, 0x00, 0x01, 0xAB, 0x8C);

#undef CHECK

    PASS();
}

TEST test_start_code_3b(void) {
    ASSERT_EQ(0, smolrtsp_test_start_code_3b(U8Slice99_empty()));

#define CHECK(expected, ...)                                                   \
    ASSERT_EQ(                                                                 \
        expected,                                                              \
        smolrtsp_test_start_code_3b(                                           \
            (U8Slice99)Slice99_typed_from_array((uint8_t[]){__VA_ARGS__})))

    CHECK(0, 0x00);
    CHECK(0, 0x00, 0x00);
    CHECK(3, 0x00, 0x00, 0x01);
    CHECK(3, 0x00, 0x00, 0x01, 0xAB);
    CHECK(3, 0x00, 0x00, 0x01, 0xAB, 0x8C);

#undef CHECK

    PASS();
}

TEST test_start_code_4b(void) {
    ASSERT_EQ(0, smolrtsp_test_start_code_4b(U8Slice99_empty()));

#define CHECK(expected, ...)                                                   \
    ASSERT_EQ(                                                                 \
        expected,                                                              \
        smolrtsp_test_start_code_4b(                                           \
            (U8Slice99)Slice99_typed_from_array((uint8_t[]){__VA_ARGS__})))

    CHECK(0, 0x00);
    CHECK(0, 0x00, 0x00);
    CHECK(4, 0x00, 0x00, 0x00, 0x01);
    CHECK(4, 0x00, 0x00, 0x00, 0x01, 0xAB);
    CHECK(4, 0x00, 0x00, 0x00, 0x01, 0xAB, 0x8C);

#undef CHECK

    PASS();
}

SUITE(nal) {
    RUN_TEST(header_unit_type_h264);
    RUN_TEST(header_unit_type_h265);
    RUN_TEST(header_size_h264);
    RUN_TEST(header_size_h265);
    RUN_TEST(header_fu_size_h264);
    RUN_TEST(header_fu_size_h265);

    RUN_TEST(header_serialize_h264);
    RUN_TEST(header_serialize_h265);
    RUN_TEST(header_serialize_fu_h264);
    RUN_TEST(header_serialize_fu_h265);

    RUN_TEST(determine_start_code);
    RUN_TEST(test_start_code_3b);
    RUN_TEST(test_start_code_4b);
}
