#include <smolrtsp/util.h>

#include "parsing.h"

#include <assert.h>
#include <stdio.h>

const char *SmolRTSP_LowerTransport_str(SmolRTSP_LowerTransport self) {
    switch (self) {
    case SmolRTSP_LowerTransport_TCP: return "TCP";
    default: return "UDP";
    }
}

int SmolRTSP_parse_lower_transport(SmolRTSP_LowerTransport *restrict result, CharSlice99 value) {
    assert(result);

    const CharSlice99 start = value;

    if (smolrtsp_match_until_str(&value, ";") != SmolRTSP_DeserializeResult_Ok) {
        return -1;
    }

    const CharSlice99 transport_specifier =
        CharSlice99_from_ptrdiff(start.ptr, (char *)value.ptr - 1);

    if (CharSlice99_primitive_ends_with(transport_specifier, CharSlice99_from_str("UDP"))) {
        *result = SmolRTSP_LowerTransport_UDP;
    } else if (CharSlice99_primitive_ends_with(transport_specifier, CharSlice99_from_str("TCP"))) {
        *result = SmolRTSP_LowerTransport_TCP;
    } else if (CharSlice99_primitive_starts_with(
                   transport_specifier, CharSlice99_from_str("RTP/AVP"))) {
        *result = SmolRTSP_LowerTransport_UDP;
    } else {
        return -1;
    }

    return 0;
}

int SmolRTSP_parse_client_port(int *restrict rtp_port, int *restrict rtcp_port, CharSlice99 value) {
    assert(rtp_port);
    assert(rtcp_port);

    if (smolrtsp_match_until_str(&value, "client_port=") != SmolRTSP_DeserializeResult_Ok) {
        return -1;
    }

    const CharSlice99 rtp_port_start = value;

    if (smolrtsp_match_numeric(&value) != SmolRTSP_DeserializeResult_Ok) {
        return -1;
    }
    if (smolrtsp_match_char(&value, '-') != SmolRTSP_DeserializeResult_Ok) {
        return -1;
    }

    const CharSlice99 rtcp_port_start = value;

    int rtp_port_temp, rtcp_port_temp;
    if (sscanf(CharSlice99_c_str(rtp_port_start, (char[128]){0}), "%d", &rtp_port_temp) != 1) {
        return -1;
    }
    if (sscanf(CharSlice99_c_str(rtcp_port_start, (char[128]){0}), "%d", &rtcp_port_temp) != 1) {
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

    if (smolrtsp_match_until_str(&value, "interleaved=") != SmolRTSP_DeserializeResult_Ok) {
        return -1;
    }

    const CharSlice99 rtp_chn_id_start = value;
    if (smolrtsp_match_numeric(&value) == SmolRTSP_DeserializeResult_Err) {
        return -1;
    }

    int rtp_port_temp;
    if (sscanf(CharSlice99_c_str(rtp_chn_id_start, (char[128]){0}), "%d", &rtp_port_temp) != 1) {
        return -1;
    }

    int rtcp_port_temp = -1;
    if (smolrtsp_match_char(&value, '-') == SmolRTSP_DeserializeResult_Ok) {
        const CharSlice99 rtcp_chn_id_start = value;

        if (sscanf(CharSlice99_c_str(rtcp_chn_id_start, (char[128]){0}), "%d", &rtcp_port_temp) !=
            1) {
            return -1;
        }
    }

    *rtp_chn_id = rtp_port_temp;
    *rtcp_chn_id = rtcp_port_temp;

    return 0;
}
