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
