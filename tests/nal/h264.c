#include <smolrtsp/nal/h264.h>

#include <greatest.h>

TEST parse(void) {
    const SmolRTSP_H264NalHeader h = SmolRTSP_H264NalHeader_parse(0b01011010);

    ASSERT(!h.forbidden_zero_bit);
    ASSERT_EQ(0b10, h.ref_idc);
    ASSERT_EQ(0b11010, h.unit_type);

    PASS();
}

TEST serialize(void) {
    const SmolRTSP_H264NalHeader h = {
        .forbidden_zero_bit = false,
        .ref_idc = 0b10,
        .unit_type = 0b11010,
    };

    const uint8_t ret = SmolRTSP_H264NalHeader_serialize(h);
    ASSERT_EQ(0b01011010, ret);

    PASS();
}

TEST write_fu_header(void) {
    const SmolRTSP_H264NalHeader h = {
        .forbidden_zero_bit = false,
        .ref_idc = 0b10,
        .unit_type = 0b11010,
    };

    uint8_t buffer[SMOLRTSP_H264_FU_HEADER_SIZE] = {0};

#define CHECK(is_first_fragment, is_last_fragment, ...)                        \
    do {                                                                       \
        SmolRTSP_H264NalHeader_write_fu_header(                                \
            h, buffer, is_first_fragment, is_last_fragment);                   \
        ASSERT_MEM_EQ(((uint8_t[]){__VA_ARGS__}), buffer, sizeof buffer);      \
    } while (0)

    CHECK(true, true, 0b01011100, 0b11011010);
    CHECK(true, false, 0b01011100, 0b10011010);
    CHECK(false, true, 0b01011100, 0b01011010);
    CHECK(false, false, 0b01011100, 0b00011010);

#undef CHECK

    PASS();
}

SUITE(nal_h264) {
    RUN_TEST(parse);
    RUN_TEST(serialize);
    RUN_TEST(write_fu_header);
}
