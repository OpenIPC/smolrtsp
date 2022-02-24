// TODO: do something with this bullshit.

#include <smolrtsp/util.h>

#include "types/parsing.h"

#include <assert.h>
#include <stdio.h>

const char *SmolRTSP_LowerTransport_str(SmolRTSP_LowerTransport self) {
    switch (self) {
    case SmolRTSP_LowerTransport_TCP:
        return "TCP";
    default:
        return "UDP";
    }
}

#define ENSURE_SUCCESS(res)                                                              \
    match(res) {                                                                         \
        of(SmolRTSP_ParseResult_Success, status) match(*status) {                        \
            of(SmolRTSP_ParseStatus_Complete, offset) value =                            \
                CharSlice99_advance(value, *offset);                                     \
            otherwise return -1;                                                         \
        }                                                                                \
        otherwise return -1;                                                             \
    }

int SmolRTSP_parse_lower_transport(
    SmolRTSP_LowerTransport *restrict result, CharSlice99 value) {
    assert(result);

    const CharSlice99 backup = value;

    const SmolRTSP_ParseResult res = smolrtsp_match_until_str(value, ";");
    ENSURE_SUCCESS(res);

    const CharSlice99 transport_specifier =
        CharSlice99_from_ptrdiff(backup.ptr, (char *)value.ptr - 1);

    if (CharSlice99_primitive_ends_with(
            transport_specifier, CharSlice99_from_str("UDP"))) {
        *result = SmolRTSP_LowerTransport_UDP;
    } else if (CharSlice99_primitive_ends_with(
                   transport_specifier, CharSlice99_from_str("TCP"))) {
        *result = SmolRTSP_LowerTransport_TCP;
    } else if (CharSlice99_primitive_starts_with(
                   transport_specifier, CharSlice99_from_str("RTP/AVP"))) {
        *result = SmolRTSP_LowerTransport_UDP;
    } else {
        return -1;
    }

    return 0;
}

int SmolRTSP_parse_client_port(
    int *restrict rtp_port, int *restrict rtcp_port, CharSlice99 value) {
    assert(rtp_port);
    assert(rtcp_port);

    SmolRTSP_ParseResult res = smolrtsp_match_until_str(value, "client_port=");
    ENSURE_SUCCESS(res);

    const CharSlice99 rtp_port_start = value;

    res = smolrtsp_match_numeric(value);
    ENSURE_SUCCESS(res);

    res = smolrtsp_match_char(value, '-');
    ENSURE_SUCCESS(res);

    const CharSlice99 rtcp_port_start = value;

    int rtp_port_temp, rtcp_port_temp;
    if (sscanf(CharSlice99_c_str(rtp_port_start, (char[128]){0}), "%d", &rtp_port_temp) !=
        1) {
        return -1;
    }
    if (sscanf(
            CharSlice99_c_str(rtcp_port_start, (char[128]){0}), "%d", &rtcp_port_temp) !=
        1) {
        return -1;
    }

    *rtp_port = rtp_port_temp;
    *rtcp_port = rtcp_port_temp;

    return 0;
}

int SmolRTSP_parse_interleaved_chn_id(
    int *restrict rtp_chn_id, int *restrict rtcp_chn_id, CharSlice99 value) {
    assert(rtp_chn_id);
    assert(rtcp_chn_id);

    SmolRTSP_ParseResult res = smolrtsp_match_until_str(value, "interleaved=");
    ENSURE_SUCCESS(res);

    const CharSlice99 rtp_chn_id_start = value;

    res = smolrtsp_match_numeric(value);

    // If the result is partial, it means we have reached the end of the string. It is
    // okay since the value can be something like `RTP/AVP/UDP;unicast;interleaved=204`.
    ifLet(res, SmolRTSP_ParseResult_Success, status) {
        *status = SmolRTSP_ParseStatus_Complete(0);
    }

    ENSURE_SUCCESS(res);

    int rtp_port_temp;
    if (sscanf(
            CharSlice99_c_str(rtp_chn_id_start, (char[128]){0}), "%d", &rtp_port_temp) !=
        1) {
        return -1;
    }

    int rtcp_port_temp = -1;
    res = smolrtsp_match_until_str(value, "-");

    // clang-format off
    ifLet(res, SmolRTSP_ParseResult_Success, status) {
        ifLet (*status, SmolRTSP_ParseStatus_Complete, offset) {
            value = CharSlice99_advance(value, *offset);
            const CharSlice99 rtcp_chn_id_start = value;

            if (sscanf(
                    CharSlice99_c_str(rtcp_chn_id_start, (char[128]){0}), "%d", &rtcp_port_temp) !=
                1) {
                return -1;
            }
        }
    }
// clang-format on

*rtp_chn_id = rtp_port_temp;
*rtcp_chn_id = rtcp_port_temp;

return 0;
}

#define DOLLAR 0x24

uint32_t SmolRTSP_InterleavedDataHeader_as_u32(SmolRTSP_InterleavedDataHeader self) {
    union {
        uint32_t n;
        uint8_t bytes[sizeof(uint32_t)];
    } repr;

    repr.bytes[0] = DOLLAR;
    repr.bytes[1] = self.channel_id;
    memcpy(repr.bytes + 2, &self.payload_len, sizeof self.payload_len);

    return repr.n;
}

SmolRTSP_InterleavedDataHeader SmolRTSP_InterleavedDataHeader_from_u32(uint32_t data) {
    union {
        uint32_t n;
        uint8_t bytes[sizeof(uint32_t)];
    } repr = {.n = data};

    assert(DOLLAR == repr.bytes[0]);

    SmolRTSP_InterleavedDataHeader self;

    self.channel_id = repr.bytes[1];

    memcpy(&self.payload_len, &repr.bytes[2], sizeof(self.payload_len));

    return self;
}

#undef DOLLAR
