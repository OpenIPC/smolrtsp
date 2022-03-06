#include <smolrtsp/ip_addr.h>

#include <string.h>

#include <arpa/inet.h>

struct sockaddr_storage
SmolRTSP_IpAddr_to_sockaddr(SmolRTSP_IpAddr self, uint16_t port) {
    struct sockaddr_storage result = {0};

    match(self) {
        of(SmolRTSP_IpAddr_V4, ipv4) {
            const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_addr = *ipv4,
                .sin_port = htons(port),
                .sin_zero = {0},
            };
            memcpy(&result, &addr, sizeof addr);
        }
        of(SmolRTSP_IpAddr_V6, ipv6) {
            const struct sockaddr_in6 addr = {
                .sin6_family = AF_INET6,
                .sin6_addr = *ipv6,
                .sin6_port = htons(port),
                .sin6_flowinfo = 0,
                .sin6_scope_id = 0,
            };
            memcpy(&result, &addr, sizeof addr);
        }
    }

    return result;
}
