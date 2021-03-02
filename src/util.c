#define SLICE99_INCLUDE_IO

#include <smolrtsp/util.h>

#include "parsing.h"

int SmolRTSP_parse_lower_transport(SmolRTSP_LowerTransport *restrict result, Slice99 value) {
    assert(result);

    const Slice99 start = value;

    if (SmolRTSP_match_until_str(&value, ";") != SmolRTSP_DeserializeResultOk) {
        return -1;
    }

    const Slice99 transport_specifier =
        Slice99_from_ptrdiff(start.ptr, (char *)value.ptr - 1, sizeof(char));

    if (Slice99_primitive_ends_with(transport_specifier, Slice99_from_str("UDP"))) {
        *result = SmolRTSP_LowerTransport_UDP;
    } else if (Slice99_primitive_ends_with(transport_specifier, Slice99_from_str("TCP"))) {
        *result = SmolRTSP_LowerTransport_TCP;
    } else if (Slice99_primitive_starts_with(transport_specifier, Slice99_from_str("RTP/AVP"))) {
        *result = SmolRTSP_LowerTransport_UDP;
    } else {
        return -1;
    }

    return 0;
}

int SmolRTSP_parse_client_port(int *restrict rtp_port, int *restrict rtcp_port, Slice99 value) {
    assert(rtp_port);
    assert(rtcp_port);

    if (SmolRTSP_match_until_str(&value, "client_port=") != SmolRTSP_DeserializeResultOk) {
        return -1;
    }

    const Slice99 rtp_port_start = value;

    if (SmolRTSP_match_numeric(&value) != SmolRTSP_DeserializeResultOk) {
        return -1;
    }
    if (SmolRTSP_match_char(&value, '-') != SmolRTSP_DeserializeResultOk) {
        return -1;
    }

    const Slice99 rtcp_port_start = value;

    int rtp_port_temp, rtcp_port_temp;
    if (sscanf(Slice99_c_str(rtp_port_start, (char[128]){0}), "%d", &rtp_port_temp) != 1) {
        return -1;
    }
    if (sscanf(Slice99_c_str(rtcp_port_start, (char[128]){0}), "%d", &rtcp_port_temp) != 1) {
        return -1;
    }

    *rtp_port = rtp_port_temp;
    *rtcp_port = rtcp_port_temp;

    return 0;
}
