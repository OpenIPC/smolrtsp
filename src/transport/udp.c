#include <smolrtsp/transport.h>

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_RETRANSMITS 10

typedef struct {
    int fd;
    char addr[128];
    size_t len;
} SmolRTSP_UdpTransport;

declImpl(SmolRTSP_Transport, SmolRTSP_UdpTransport);

static int send_packet(SmolRTSP_UdpTransport *self, struct msghdr message);
static int
new_sockaddr(struct sockaddr *addr, int af, const void *ip, uint16_t port);

SmolRTSP_Transport smolrtsp_transport_udp(int fd) {
    assert(fd >= 0);

    SmolRTSP_UdpTransport *self = malloc(sizeof *self);
    assert(self);
    self->fd = fd;
    self->len = 0;

    return DYN(SmolRTSP_UdpTransport, SmolRTSP_Transport, self);
}

SmolRTSP_Transport
smolrtsp_transport_udp_address(int fd, void *addr, size_t len) {
    assert(fd >= 0);
    assert(addr && len > 0);

    SmolRTSP_UdpTransport *self = malloc(sizeof *self);
    assert(self);
    self->fd = fd;
    self->len = len;
    memcpy(&self->addr, addr, len);

    return DYN(SmolRTSP_UdpTransport, SmolRTSP_Transport, self);
}

static void SmolRTSP_UdpTransport_drop(VSelf) {
    VSELF(SmolRTSP_UdpTransport);
    assert(self);

    free(self);
}

impl(SmolRTSP_Droppable, SmolRTSP_UdpTransport);

static int SmolRTSP_UdpTransport_transmit(VSelf, SmolRTSP_IoVecSlice bufs) {
    VSELF(SmolRTSP_UdpTransport);
    assert(self);

    const struct msghdr msg = {
        .msg_name = self->addr,
        .msg_namelen = self->len,
        .msg_iov = bufs.ptr,
        .msg_iovlen = bufs.len,
        .msg_control = NULL,
        .msg_controllen = 0,
        .msg_flags = 0,
    };

    return send_packet(self, msg);
}

static bool SmolRTSP_UdpTransport_is_full(VSelf) {
    VSELF(SmolRTSP_UdpTransport);
    (void)self;

    return false;
}

impl(SmolRTSP_Transport, SmolRTSP_UdpTransport);

static int send_packet(SmolRTSP_UdpTransport *self, struct msghdr message) {
    // Try to retransmit a packet several times on `EMSGSIZE`. The kernel
    // will fragment an IP packet because if `IP_PMTUDISC_WANT` is set.
    size_t i = MAX_RETRANSMITS;
    do {
        const ssize_t ret = sendmsg(self->fd, &message, 0);

        if (ret != -1) {
            return 0;
        }
        if (EMSGSIZE != errno) {
            return -1;
        }

        assert(-1 == ret);
        assert(EMSGSIZE == errno);

        // Try to retransmit the packet one more time.
        i--;
    } while (i > 0);

    return -1;
}

int smolrtsp_dgram_socket(int af, const void *restrict addr, uint16_t port) {
    struct sockaddr_storage dest;
    memset(&dest, '\0', sizeof dest);
    if (new_sockaddr((struct sockaddr *)&dest, af, addr, port) == -1) {
        return -1;
    }

    int fd;
    if ((fd = socket(af, SOCK_DGRAM, 0)) == -1) {
        goto fail;
    }

    if (connect(
            fd, (const struct sockaddr *)&dest,
            AF_INET == af ? sizeof(struct sockaddr_in)
                          : sizeof(struct sockaddr_in6)) == -1) {
        perror("connect");
        goto fail;
    }

    const int enable_pmtud = 1;
    if (setsockopt(
            fd, IPPROTO_IP, IP_PMTUDISC_WANT, &enable_pmtud,
            sizeof enable_pmtud) == -1) {
        perror("setsockopt IP_PMTUDISC_WANT");
        goto fail;
    }

    return fd;

fail:
    close(fd);
    return -1;
}

static int
new_sockaddr(struct sockaddr *addr, int af, const void *ip, uint16_t port) {
    switch (af) {
    case AF_INET: {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
        addr_in->sin_family = AF_INET;
        memcpy(&addr_in->sin_addr, ip, sizeof(struct in_addr));
        addr_in->sin_port = htons(port);
        return 0;
    }
    case AF_INET6: {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)addr;
        addr_in6->sin6_family = AF_INET6;
        memcpy(&addr_in6->sin6_addr, ip, sizeof(struct in6_addr));
        addr_in6->sin6_port = htons(port);
        return 0;
    }
    default:
        errno = EAFNOSUPPORT;
        return -1;
    }
}

void *smolrtsp_sockaddr_ip(const struct sockaddr *restrict addr) {
    assert(addr);

    switch (addr->sa_family) {
    case AF_INET:
        return (void *)&((struct sockaddr_in *)addr)->sin_addr;
    case AF_INET6:
        return (void *)&((struct sockaddr_in6 *)addr)->sin6_addr;
    default:
        return NULL;
    }
}
