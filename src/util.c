#include <smolrtsp/util.h>

#include <assert.h>
#include <inttypes.h>
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

int smolrtsp_parse_range(
    void *restrict num_0, void *restrict num_1,
    const char *restrict num_specifier, const char *restrict param_name,
    CharSlice99 header_value) {
    assert(num_0);
    assert(num_1);
    assert(num_specifier);
    assert(param_name);

    CharSlice99 range;
    if (smolrtsp_parse_header_param(&range, param_name, header_value) == -1) {
        return -1;
    }

    char fmt[16] = {0};
    snprintf(fmt, sizeof fmt, "%%%s-%%%s", num_specifier, num_specifier);

    char *port_pair_str = CharSlice99_alloca_c_str(range);
    if (sscanf(port_pair_str, fmt, num_0, num_1) != 2) {
        return -1;
    }

    return 0;
}

int smolrtsp_parse_port_pair(
    SmolRTSP_PortPair *restrict pair, const char *restrict param_name,
    CharSlice99 header_value) {
    assert(pair);
    assert(param_name);

    if (smolrtsp_parse_range(
            &pair->rtp_port, &pair->rtcp_port, SCNu16, param_name,
            header_value) == -1) {
        return -1;
    }

    return 0;
}

int smolrtsp_parse_header_param(
    CharSlice99 *restrict param_value, const char *restrict param_name,
    CharSlice99 header_value) {
    assert(param_name);
    assert(param_value);

    char *header_value_str =
        CharSlice99_alloca_c_str(header_value); // for strstr/strchr

    char *param_name_ptr = strstr(header_value_str, param_name);
    if (NULL == param_name_ptr) {
        return -1;
    }

    char *semicolon_ptr = strchr(param_name_ptr, ';');
    const ptrdiff_t start_idx =
        (param_name_ptr - header_value_str) + strlen(param_name);
    ptrdiff_t end_idx = header_value.len;

    if (NULL != semicolon_ptr) {
        end_idx = semicolon_ptr - header_value_str;
    }

    *param_value = CharSlice99_sub(header_value, start_idx, end_idx);
    return 0;
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
