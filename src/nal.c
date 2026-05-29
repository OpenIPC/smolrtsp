#include <smolrtsp/nal.h>

#include <assert.h>

/* Per-codec match arm fragments. Each expands to either the corresponding
 * `of(...)` arm or nothing, depending on the SMOLRTSP_WITH_HXXX gates. They
 * are top-level macros (not nested in the derive macros below) so that the
 * preprocessor evaluates the gates before the derive macro is expanded —
 * #ifdef cannot legally appear inside a macro's replacement-list. */
#ifdef SMOLRTSP_WITH_H264
#define SMOLRTSP_PRIV_NAL_H264_ARM(body) of(SmolRTSP_NalHeader_H264, h) body
#else
#define SMOLRTSP_PRIV_NAL_H264_ARM(body)
#endif
#ifdef SMOLRTSP_WITH_H265
#define SMOLRTSP_PRIV_NAL_H265_ARM(body) of(SmolRTSP_NalHeader_H265, h) body
#else
#define SMOLRTSP_PRIV_NAL_H265_ARM(body)
#endif
#ifdef SMOLRTSP_WITH_H266
#define SMOLRTSP_PRIV_NAL_H266_ARM(body) of(SmolRTSP_NalHeader_H266, h) body
#else
#define SMOLRTSP_PRIV_NAL_H266_ARM(body)
#endif

#define NAL_HEADER_DERIVE_GETTER(T, name, h264_value, h265_value, h266_value)  \
    T SmolRTSP_NalHeader_##name(SmolRTSP_NalHeader self) {                     \
        T result = 0;                                                          \
                                                                               \
        match(self) {                                                          \
            SMOLRTSP_PRIV_NAL_H264_ARM(result = h264_value;)                   \
            SMOLRTSP_PRIV_NAL_H265_ARM(result = h265_value;)                   \
            SMOLRTSP_PRIV_NAL_H266_ARM(result = h266_value;)                   \
        }                                                                      \
                                                                               \
        return result;                                                         \
    }

NAL_HEADER_DERIVE_GETTER(
    uint8_t, unit_type, h->unit_type, h->unit_type, h->unit_type)
NAL_HEADER_DERIVE_GETTER(
    size_t, size, ((void)h, SMOLRTSP_H264_NAL_HEADER_SIZE),
    ((void)h, SMOLRTSP_H265_NAL_HEADER_SIZE),
    ((void)h, SMOLRTSP_H266_NAL_HEADER_SIZE))
NAL_HEADER_DERIVE_GETTER(
    size_t, fu_size, ((void)h, SMOLRTSP_H264_FU_HEADER_SIZE),
    ((void)h, SMOLRTSP_H265_FU_HEADER_SIZE),
    ((void)h, SMOLRTSP_H266_FU_HEADER_SIZE))

#undef NAL_HEADER_DERIVE_GETTER

#define NAL_HEADER_DERIVE_PREDICATE(fn)                                        \
    bool SmolRTSP_NalHeader_##fn(SmolRTSP_NalHeader self) {                    \
        bool result = 0;                                                       \
                                                                               \
        match(self) {                                                          \
            SMOLRTSP_PRIV_NAL_H264_ARM(                                        \
                result = SmolRTSP_H264NalHeader_##fn(*h);)                     \
            SMOLRTSP_PRIV_NAL_H265_ARM(                                        \
                result = SmolRTSP_H265NalHeader_##fn(*h);)                     \
            SMOLRTSP_PRIV_NAL_H266_ARM(                                        \
                result = SmolRTSP_H266NalHeader_##fn(*h);)                     \
        }                                                                      \
                                                                               \
        return result;                                                         \
    }

NAL_HEADER_DERIVE_PREDICATE(is_vps)
NAL_HEADER_DERIVE_PREDICATE(is_sps)
NAL_HEADER_DERIVE_PREDICATE(is_pps)
NAL_HEADER_DERIVE_PREDICATE(is_coded_slice_idr)
NAL_HEADER_DERIVE_PREDICATE(is_coded_slice_non_idr)

#undef NAL_HEADER_DERIVE_PREDICATE

void SmolRTSP_NalHeader_serialize(
    SmolRTSP_NalHeader self, uint8_t buffer[restrict]) {
    match(self) {
#ifdef SMOLRTSP_WITH_H264
        of(SmolRTSP_NalHeader_H264, h) {
            const uint8_t repr = SmolRTSP_H264NalHeader_serialize(*h);
            buffer[0] = repr;
        }
#endif
#ifdef SMOLRTSP_WITH_H265
        of(SmolRTSP_NalHeader_H265, h) {
            const uint16_t repr = SmolRTSP_H265NalHeader_serialize(*h);
            memcpy(buffer, &repr, sizeof repr);
        }
#endif
#ifdef SMOLRTSP_WITH_H266
        of(SmolRTSP_NalHeader_H266, h) {
            const uint16_t repr = SmolRTSP_H266NalHeader_serialize(*h);
            memcpy(buffer, &repr, sizeof repr);
        }
#endif
    }
}

void SmolRTSP_NalHeader_write_fu_header(
    SmolRTSP_NalHeader self, uint8_t buffer[restrict], bool is_first_fragment,
    bool is_last_fragment) {
    match(self) {
#ifdef SMOLRTSP_WITH_H264
        of(SmolRTSP_NalHeader_H264, h) SmolRTSP_H264NalHeader_write_fu_header(
            *h, buffer, is_first_fragment, is_last_fragment);
#endif
#ifdef SMOLRTSP_WITH_H265
        of(SmolRTSP_NalHeader_H265, h) SmolRTSP_H265NalHeader_write_fu_header(
            *h, buffer, is_first_fragment, is_last_fragment);
#endif
#ifdef SMOLRTSP_WITH_H266
        of(SmolRTSP_NalHeader_H266, h) SmolRTSP_H266NalHeader_write_fu_header(
            *h, buffer, is_first_fragment, is_last_fragment);
#endif
    }
}

// See <https://tools.ietf.org/html/rfc7798#section-4.4.3> (H.265),
// <https://tools.ietf.org/html/rfc6184#section-5.8> (H.264).
uint8_t smolrtsp_nal_fu_header(
    bool is_first_fragment, bool is_last_fragment, uint8_t unit_type) {
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

static const uint8_t start_code_3b[] = {0x00, 0x00, 0x01},
                     start_code_4b[] = {0x00, 0x00, 0x00, 0x01};

SmolRTSP_NalStartCodeTester smolrtsp_determine_start_code(U8Slice99 data) {
    if (smolrtsp_test_start_code_3b(data)) {
        return smolrtsp_test_start_code_3b;
    }

    if (smolrtsp_test_start_code_4b(data)) {
        return smolrtsp_test_start_code_4b;
    }

    return NULL;
}

size_t smolrtsp_test_start_code_3b(U8Slice99 data) {
    if (data.len < 3) {
        return 0;
    }
    if (memcmp(data.ptr, start_code_3b, 3) == 0) {
        return 3;
    }

    return 0;
}

size_t smolrtsp_test_start_code_4b(U8Slice99 data) {
    if (data.len < 4) {
        return 0;
    }
    if (memcmp(data.ptr, start_code_4b, 4) == 0) {
        return 4;
    }

    return 0;
}
