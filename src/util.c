// TODO: do something with this bullshit.

#include <smolrtsp/util.h>

#include "types/parsing.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <alloca.h>

const char *SmolRTSP_LowerTransport_str(SmolRTSP_LowerTransport self) {
    switch (self) {
    case SmolRTSP_LowerTransport_TCP:
        return "TCP";
    case SmolRTSP_LowerTransport_UDP:
        return "UDP";
    default:
        return "Unknown";
    }
}

int smolrtsp_parse_lower_transport(CharSlice99 value) {
    if (CharSlice99_primitive_starts_with(
            value, CharSlice99_from_str("RTP/AVP/UDP"))) {
        return SmolRTSP_LowerTransport_UDP;
    } else if (CharSlice99_primitive_starts_with(
                   value, CharSlice99_from_str("RTP/AVP/TCP"))) {
        return SmolRTSP_LowerTransport_TCP;
    } else if (CharSlice99_primitive_starts_with(
                   value, CharSlice99_from_str("RTP/AVP"))) {
        return SmolRTSP_LowerTransport_UDP;
    } else {
        return -1;
    }
}

bool smolrtsp_parse_header_param(
    const char *restrict param_name, CharSlice99 value,
    CharSlice99 *restrict param_value) {
    assert(param_name);
    assert(param_value);

    // Make a null-terminated string to use `strstr`.
    char *value_str = alloca(value.len + 1);
    memcpy(value_str, value.ptr, value.len);
    value_str[value.len] = '\0';

    char *param_name_ptr = strstr(value_str, param_name);
    if (NULL == param_name_ptr) {
        return false;
    }

    char *semicolon_ptr = strchr(param_name_ptr, ';');
    const ptrdiff_t start_idx =
        (param_name_ptr - value_str) + strlen(param_name);
    ptrdiff_t end_idx = value.len;

    if (NULL != semicolon_ptr) {
        end_idx = semicolon_ptr - value_str;
    }

    *param_value = CharSlice99_sub(value, start_idx, end_idx);
    return true;
}

uint32_t smolrtsp_interleaved_header(uint8_t channel_id, uint16_t payload_len) {
    uint8_t bytes[sizeof(uint32_t)] = {0};

    bytes[0] = '$';
    bytes[1] = channel_id;
    memcpy(bytes + 2, &payload_len, sizeof payload_len);

    uint32_t n;
    memcpy(&n, bytes, sizeof bytes);
    return n;
}

void smolrtsp_parse_interleaved_header(
    uint32_t data, uint8_t *restrict channel_id,
    uint16_t *restrict payload_len) {
    assert(channel_id);
    assert(data);

    uint8_t bytes[sizeof(uint32_t)] = {0};
    memcpy(bytes, &data, sizeof data);

    assert('$' == bytes[0]);

    *channel_id = bytes[1];
    memcpy(payload_len, &bytes[2], sizeof *payload_len);
}
