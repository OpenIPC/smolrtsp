#include <smolrtsp/util.h>

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <alloca.h>
#include <arpa/inet.h>

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

#define PARSE_RANGE(lhs, rhs, specifier, param)                                \
    sscanf((param), "%*[^=]=%" specifier "-%" specifier, &(lhs), &(rhs))
#define STARTS_WITH(str, beginning)                                            \
    (strncmp((str), (beginning), strlen(beginning)) == 0)

static int parse_lower_transport(
    SmolRTSP_TransportConfig *restrict result, const char *header_value);
static int parse_transport_param(
    SmolRTSP_TransportConfig *restrict result, const char *param);
static int parse_channel_pair(
    SmolRTSP_ChannelPair *restrict val, const char *restrict param);
static int
parse_port_pair(SmolRTSP_PortPair *restrict val, const char *restrict param);

int smolrtsp_parse_transport(
    SmolRTSP_TransportConfig *restrict config, CharSlice99 header_value) {
    assert(config);

    SmolRTSP_TransportConfig result = {
        .lower = 0,
        .unicast = false,
        .multicast = false,
        .interleaved = SmolRTSP_ChannelPair_None(),
        .client_port = SmolRTSP_PortPair_None(),
    };

    const char *input = CharSlice99_alloca_c_str(header_value);

    if (parse_lower_transport(&result, input) == -1) {
        return -1;
    }
    if ((input = strchr(input, ';')) == NULL) {
        goto success;
    }
    input++;

    while (input != NULL) {
        const char *next_param = strchr(input, ';');
        const CharSlice99 param =
            NULL == next_param
                ? CharSlice99_from_str((char *)input)
                : CharSlice99_from_ptrdiff((char *)input, (char *)next_param);

        if (parse_transport_param(&result, CharSlice99_alloca_c_str(param)) ==
            -1) {
            return -1;
        }

        input = next_param != NULL ? next_param + 1 : NULL;
    }

success:
    *config = result;
    return 0;
}

static int parse_lower_transport(
    SmolRTSP_TransportConfig *restrict result, const char *header_value) {
    const char *rtp_avp = "RTP/AVP";
    if (strncmp(header_value, rtp_avp, strlen(rtp_avp)) != 0) {
        return -1;
    }
    header_value += strlen(rtp_avp);

    if (strncmp(header_value, "/TCP", strlen("/TCP")) == 0) {
        result->lower = SmolRTSP_LowerTransport_TCP;
    } else if (strncmp(header_value, "/UDP", strlen("/UDP")) == 0) {
        result->lower = SmolRTSP_LowerTransport_UDP;
    } else {
        result->lower = SmolRTSP_LowerTransport_UDP;
    }

    return 0;
}

static int parse_transport_param(
    SmolRTSP_TransportConfig *restrict result, const char *param) {
    if (STARTS_WITH(param, "unicast")) {
        result->unicast = true;
    } else if (STARTS_WITH(param, "multicast")) {
        result->multicast = true;
    } else if (STARTS_WITH(param, "interleaved")) {
        SmolRTSP_ChannelPair val;
        if (parse_channel_pair(&val, param) == -1) {
            return -1;
        }
        result->interleaved = SmolRTSP_ChannelPair_Some(val);
    } else if (STARTS_WITH(param, "client_port")) {
        SmolRTSP_PortPair val;
        if (parse_port_pair(&val, param) == -1) {
            return -1;
        }
        result->client_port = SmolRTSP_PortPair_Some(val);
    }

    return 0;
}

static int parse_channel_pair(
    SmolRTSP_ChannelPair *restrict val, const char *restrict param) {
    if (PARSE_RANGE(val->rtp_channel, val->rtcp_channel, SCNu8, param) < 1) {
        return -1;
    }

    return 0;
}

static int
parse_port_pair(SmolRTSP_PortPair *restrict val, const char *restrict param) {
    if (PARSE_RANGE(val->rtp_port, val->rtcp_port, SCNu16, param) < 1) {
        return -1;
    }

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
    const uint8_t data[restrict static 4], uint8_t *restrict channel_id,
    uint16_t *restrict payload_len) {
    assert(channel_id);
    assert(payload_len);
    assert('$' == data[0]);

    *channel_id = data[1];
    memcpy(payload_len, data + 2, sizeof *payload_len);
    *payload_len = ntohs(*payload_len);
}
