#include <smolrtsp/transport.h>

#include <greatest.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>

#define DATA_0 "abc"
#define DATA_1 "defghi"

static enum greatest_test_res test_transport(
    SmolRTSP_Transport t, int read_fd, size_t len,
    const char expected[restrict static len]) {
    struct iovec bufs[] = {
        {.iov_base = DATA_0, .iov_len = sizeof((char[]){DATA_0}) - 1},
        {.iov_base = DATA_1, .iov_len = sizeof((char[]){DATA_1}) - 1},
    };

    const SmolRTSP_IoVecSlice slices = Slice99_typed_from_array(bufs);

    const ssize_t ret = VCALL(t, transmit, slices);
    ASSERT_EQ(0, ret);

    char *buffer = malloc(len);
    read(read_fd, buffer, len);
    ASSERT_MEM_EQ(expected, buffer, len);
    free(buffer);

    VCALL_SUPER(t, SmolRTSP_Droppable, drop);

    PASS();
}

TEST check_tcp(void) {
    int fds[2];
    const bool socketpair_ok = socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0;
    ASSERT(socketpair_ok);

    const uint8_t chn_id = 123;

    SmolRTSP_Transport tcp =
        smolrtsp_transport_tcp(smolrtsp_fd_writer(&fds[0]), chn_id, 0);

    const char total_len = strlen(DATA_0) + strlen(DATA_1);
    const char expected[] = {'$', chn_id, 0,   total_len, 'a', 'b', 'c',
                             'd', 'e',    'f', 'g',       'h', 'i'};

    CHECK_CALL(test_transport(tcp, fds[1], sizeof expected, expected));

    close(fds[0]);
    close(fds[1]);
    PASS();
}

TEST check_udp(void) {
    int fds[2];
    const bool socketpair_ok = socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fds) == 0;
    ASSERT(socketpair_ok);

    SmolRTSP_Transport udp = smolrtsp_transport_udp(fds[0]);

    char expected[] = {DATA_0 DATA_1};

    CHECK_CALL(test_transport(udp, fds[1], strlen(expected), expected));

    close(fds[0]);
    close(fds[1]);
    PASS();
}

TEST sockaddr_get_ipv4(void) {
    struct sockaddr_storage addr;
    memset(&addr, '\0', sizeof addr);
    addr.ss_family = AF_INET;

    ASSERT_EQ(
        (void *)&((struct sockaddr_in *)&addr)->sin_addr,
        smolrtsp_sockaddr_ip((const struct sockaddr *)&addr));

    PASS();
}

TEST sockaddr_get_ipv6(void) {
    struct sockaddr_storage addr;
    memset(&addr, '\0', sizeof addr);
    addr.ss_family = AF_INET6;

    ASSERT_EQ(
        (void *)&((struct sockaddr_in6 *)&addr)->sin6_addr,
        smolrtsp_sockaddr_ip((const struct sockaddr *)&addr));

    PASS();
}

TEST sockaddr_get_unknown(void) {
    struct sockaddr_storage addr;
    memset(&addr, '\0', sizeof addr);
    addr.ss_family = AF_UNIX;

    ASSERT_EQ(NULL, smolrtsp_sockaddr_ip((const struct sockaddr *)&addr));

    PASS();
}

SUITE(transport) {
    RUN_TEST(check_tcp);
    RUN_TEST(check_udp);
    RUN_TEST(sockaddr_get_ipv4);
    RUN_TEST(sockaddr_get_ipv6);
    RUN_TEST(sockaddr_get_unknown);
}
