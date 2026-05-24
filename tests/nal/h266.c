#include <smolrtsp/nal/h266.h>

#include <greatest.h>

TEST parse(void) {
    /* byte 0: F(0) Z(0) LayerId(010101) = 0b00010101 = 0x15
     * byte 1: Type(10010) TID(101)      = 0b10010101 = 0x95 */
    const SmolRTSP_H266NalHeader h =
        SmolRTSP_H266NalHeader_parse((uint8_t[]){0x15, 0x95});

    ASSERT(!h.forbidden_zero_bit);
    ASSERT(!h.nuh_reserved_zero_bit);
    ASSERT_EQ(0b010101, h.nuh_layer_id);
    ASSERT_EQ(0b10010, h.unit_type);
    ASSERT_EQ(0b101, h.nuh_temporal_id_plus1);

    PASS();
}

TEST serialize(void) {
    const SmolRTSP_H266NalHeader h = {
        .forbidden_zero_bit = false,
        .nuh_reserved_zero_bit = false,
        .nuh_layer_id = 0b010101,
        .unit_type = 0b10010,
        .nuh_temporal_id_plus1 = 0b101,
    };

    const uint16_t ret = SmolRTSP_H266NalHeader_serialize(h);
    /* Little-endian on this host: bytes in memory are 0x15 then 0x95, so
     * the resulting uint16_t value loaded with memcpy is 0x9515. The wire
     * order is what callers actually observe via the byte buffer (tested
     * separately via the FU header test). */
    ASSERT_EQ(0b1001010100010101, ret);

    PASS();
}

TEST write_fu_header(void) {
    const SmolRTSP_H266NalHeader h = {
        .forbidden_zero_bit = false,
        .nuh_reserved_zero_bit = false,
        .nuh_layer_id = 0b010101,
        .unit_type = 0b10010,
        .nuh_temporal_id_plus1 = 0b101,
    };

    uint8_t buffer[SMOLRTSP_H266_FU_HEADER_SIZE] = {0};

#define CHECK(is_first_fragment, is_last_fragment, ...)                        \
    do {                                                                       \
        SmolRTSP_H266NalHeader_write_fu_header(                                \
            h, buffer, is_first_fragment, is_last_fragment);                   \
        ASSERT_MEM_EQ(((uint8_t[]){__VA_ARGS__}), buffer, sizeof buffer);      \
    } while (0)

    /* Payload header byte 0: F(0) Z(0) LayerId(010101) = 0b00010101 = 0x15
     * Payload header byte 1: Type(11101 = FU=29) TID(101) = 0b11101101 = 0xED
     * FU header byte: S | E | original_unit_type5 (bits 3-7) */
    CHECK(true, true, 0x15, 0xED, 0b11010010);
    CHECK(true, false, 0x15, 0xED, 0b10010010);
    CHECK(false, true, 0x15, 0xED, 0b01010010);
    CHECK(false, false, 0x15, 0xED, 0b00010010);

#undef CHECK

    PASS();
}

SUITE(nal_h266) {
    RUN_TEST(parse);
    RUN_TEST(serialize);
    RUN_TEST(write_fu_header);
}
