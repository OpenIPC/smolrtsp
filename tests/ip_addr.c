#include <smolrtsp/ip_addr.h>

#include <greatest.h>

#include <arpa/inet.h>
#include <stdbool.h>

TEST to_sockaddr_v4(void) {
    struct in_addr addr = {.s_addr = inet_addr("145.92.1.243")};
    const SmolRTSP_IpAddr ip = SmolRTSP_IpAddr_V4(addr);
    const uint16_t port = 554;

    const struct sockaddr_storage storage =
        SmolRTSP_IpAddr_to_sockaddr(ip, port);
    const struct sockaddr_in *result = (const struct sockaddr_in *)&storage;

    ASSERT_EQ(AF_INET, result->sin_family);
    ASSERT_MEM_EQ(&addr, &result->sin_addr, sizeof addr);
    ASSERT_EQ(port, htons(result->sin_port));

    PASS();
}

TEST to_sockaddr_v6(void) {
    struct in6_addr addr;
    const bool inet_pton_ok =
        inet_pton(AF_INET6, "2001:db8:3333:4444:5555:6666:7777:8888", &addr) ==
        1;
    ASSERT(inet_pton_ok);

    const SmolRTSP_IpAddr ip = SmolRTSP_IpAddr_V6(addr);
    const uint16_t port = 554;

    const struct sockaddr_storage storage =
        SmolRTSP_IpAddr_to_sockaddr(ip, port);
    const struct sockaddr_in6 *result = (const struct sockaddr_in6 *)&storage;

    ASSERT_EQ(AF_INET6, result->sin6_family);
    ASSERT_MEM_EQ(&addr, &result->sin6_addr, sizeof addr);
    ASSERT_EQ(port, htons(result->sin6_port));
    ASSERT_EQ(0, result->sin6_flowinfo);
    ASSERT_EQ(0, result->sin6_scope_id);

    PASS();
}

SUITE(ip_addr) {
    RUN_TEST(to_sockaddr_v4);
    RUN_TEST(to_sockaddr_v6);
}
