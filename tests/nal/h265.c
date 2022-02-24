#include <smolrtsp/nal/h265.h>

#include <greatest.h>

TEST parse(void) {
    const SmolRTSP_H265NalHeader h =
        SmolRTSP_H265NalHeader_parse((uint8_t[]){0b01001011, 0b10101101});

    ASSERT(!h.forbidden_zero_bit);
    ASSERT_EQ(0b100101, h.unit_type);
    ASSERT_EQ(0b110101, h.nuh_layer_id);
    ASSERT_EQ(0b101, h.nuh_temporal_id_plus1);

    PASS();
}

TEST serialize(void) {
    const SmolRTSP_H265NalHeader h = {
        .forbidden_zero_bit = false,
        .unit_type = 0b100101,
        .nuh_layer_id = 0b110101,
        .nuh_temporal_id_plus1 = 0b101,
    };

    const uint16_t ret = SmolRTSP_H265NalHeader_serialize(h);
    ASSERT_EQ(0b1010110101001011, ret);

    PASS();
}

TEST write_fu_header(void) {
    const SmolRTSP_H265NalHeader h = {
        .forbidden_zero_bit = false,
        .unit_type = 0b100101,
        .nuh_layer_id = 0b110101,
        .nuh_temporal_id_plus1 = 0b101,
    };

    uint8_t buffer[SMOLRTSP_H265_FU_HEADER_SIZE] = {0};

#define CHECK(is_first_fragment, is_last_fragment, ...)                        \
    do {                                                                       \
        SmolRTSP_H265NalHeader_write_fu_header(                                \
            h, buffer, is_first_fragment, is_last_fragment);                   \
        ASSERT_MEM_EQ(((uint8_t[]){__VA_ARGS__}), buffer, sizeof buffer);      \
    } while (0)

    CHECK(true, true, 0b01100011, 0b10101101, 0b11100101);
    CHECK(true, false, 0b01100011, 0b10101101, 0b10100101);
    CHECK(false, true, 0b01100011, 0b10101101, 0b01100101);
    CHECK(false, false, 0b01100011, 0b10101101, 0b00100101);

#undef CHECK

    PASS();
}

SUITE(nal_h265) {
    RUN_TEST(parse);
    RUN_TEST(serialize);
    RUN_TEST(write_fu_header);
}
