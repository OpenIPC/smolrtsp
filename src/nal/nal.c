#include <smolrtsp/nal/nal.h>

#include <assert.h>

uint8_t SmolRTSP_NalHeader_unit_type(SmolRTSP_NalHeader self) {
    uint8_t result = 0;

    match(self) {
        of(SmolRTSP_NalHeader_H264, h) result = h->unit_type;
        of(SmolRTSP_NalHeader_H265, h) result = h->unit_type;
    }

    return result;
}

size_t SmolRTSP_NalHeader_size(SmolRTSP_NalHeader self) {
    size_t result = 0;

    match(self) {
        of(SmolRTSP_NalHeader_H264, _) result = SMOLRTSP_H264_NAL_HEADER_SIZE;
        of(SmolRTSP_NalHeader_H265, _) result = SMOLRTSP_H265_NAL_HEADER_SIZE;
    }

    return result;
}

#define NAL_HEADER_DERIVE_METHOD(fn)                                                               \
    bool SmolRTSP_NalHeader_##fn(SmolRTSP_NalHeader self) {                                        \
        bool result = 0;                                                                           \
                                                                                                   \
        match(self) {                                                                              \
            of(SmolRTSP_NalHeader_H264, h) result = SmolRTSP_H264NalHeader_##fn(*h);               \
            of(SmolRTSP_NalHeader_H265, h) result = SmolRTSP_H265NalHeader_##fn(*h);               \
        }                                                                                          \
                                                                                                   \
        return result;                                                                             \
    }

NAL_HEADER_DERIVE_METHOD(is_vps)
NAL_HEADER_DERIVE_METHOD(is_sps)
NAL_HEADER_DERIVE_METHOD(is_pps)
NAL_HEADER_DERIVE_METHOD(is_coded_slice_idr)
NAL_HEADER_DERIVE_METHOD(is_coded_slice_non_idr)

#undef NAL_HEADER_DERIVE_METHOD

void SmolRTSP_NalHeader_serialize(SmolRTSP_NalHeader self, uint8_t buffer[restrict]) {
    match(self) {
        of(SmolRTSP_NalHeader_H264, h) {
            const uint8_t repr = SmolRTSP_H264NalHeader_serialize(*h);
            buffer[0] = repr;
        }
        of(SmolRTSP_NalHeader_H265, h) {
            const uint16_t repr = SmolRTSP_H265NalHeader_serialize(*h);
            memcpy(buffer, &repr, sizeof repr);
        }
    }
}

void SmolRTSP_NalHeader_write_fu_header(
    SmolRTSP_NalHeader self, uint8_t buffer[restrict], bool is_first_fragment,
    bool is_last_fragment) {
    match(self) {
        of(SmolRTSP_NalHeader_H264, h)
            SmolRTSP_H264NalHeader_write_fu_header(*h, buffer, is_first_fragment, is_last_fragment);
        of(SmolRTSP_NalHeader_H265, h)
            SmolRTSP_H265NalHeader_write_fu_header(*h, buffer, is_first_fragment, is_last_fragment);
    }
}

SmolRTSP_NalUnit SmolRTSP_NalUnit_new(SmolRTSP_NalHeader h, U8Slice99 payload) {
    SmolRTSP_NalUnit result = {0};

    match(h) {
        of(SmolRTSP_NalHeader_H264, h) result =
            SmolRTSP_NalUnit_H264((SmolRTSP_H264NalUnit){.header = *h, .payload = payload});
        of(SmolRTSP_NalHeader_H265, h) result =
            SmolRTSP_NalUnit_H265((SmolRTSP_H265NalUnit){.header = *h, .payload = payload});
    }

    return result;
}

SmolRTSP_NalHeader SmolRTSP_NalUnit_header(SmolRTSP_NalUnit self) {
    SmolRTSP_NalHeader result = {0};

    match(self) {
        of(SmolRTSP_NalUnit_H264, u) result = SmolRTSP_NalHeader_H264(u->header);
        of(SmolRTSP_NalUnit_H265, u) result = SmolRTSP_NalHeader_H265(u->header);
    }

    return result;
}

U8Slice99 SmolRTSP_NalUnit_payload(SmolRTSP_NalUnit self) {
    U8Slice99 result = {0};

    match(self) {
        of(SmolRTSP_NalUnit_H264, u) result = u->payload;
        of(SmolRTSP_NalUnit_H265, u) result = u->payload;
    }

    return result;
}

// See <https://tools.ietf.org/html/rfc7798#section-4.4.3> (H.265),
// <https://tools.ietf.org/html/rfc6184#section-5.8> (H.264).
uint8_t smolrtsp_nal_fu_header(bool is_first_fragment, bool is_last_fragment, uint8_t unit_type) {
    /*
     * H.264:
     * +---------------+
     * |0|1|2|3|4|5|6|7|
     * +-+-+-+-+-+-+-+-+
     * |S|E|R|  Type   |
     * +---------------+
     * */

    /*
     * H.265:
     * +---------------+
     * |0|1|2|3|4|5|6|7|
     * +-+-+-+-+-+-+-+-+
     * |S|E|  FuType   |
     * +---------------+
     */
    uint8_t fu_header = (uint8_t)0b00000000;

    if (is_first_fragment) {
        fu_header |= (uint8_t)0b10000000;
    }
    if (is_last_fragment) {
        fu_header |= (uint8_t)0b01000000;
    }
    fu_header += unit_type;

    return fu_header;
}

bool smolrtsp_nal_test_start_code(U8Slice99 data, U8Slice99 *restrict data_without_start_code) {
    static const uint8_t start_code_3b[] = {0x00, 0x00, 0x01},
                         start_code_4b[] = {0x00, 0x00, 0x00, 0x01};

    if (data.len < 3) {
        return false;
    }
    if (memcmp(data.ptr, start_code_3b, 3) == 0) {
        *data_without_start_code = U8Slice99_advance(data, 3);
        return true;
    }

    if (data.len < 4) {
        return false;
    }
    if (memcmp(data.ptr, start_code_4b, 4) == 0) {
        *data_without_start_code = U8Slice99_advance(data, 4);
        return true;
    }

    return false;
}
