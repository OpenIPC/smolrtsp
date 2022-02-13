#include <smolrtsp/transport.h>

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#define MAX_RETRANSMITS 10

typedef struct {
    int fd;
} SmolRTSP_UdpTransport;

declImpl(SmolRTSP_Droppable, SmolRTSP_UdpTransport);
declImpl(SmolRTSP_Transport, SmolRTSP_UdpTransport);

static int send_packet(SmolRTSP_UdpTransport *self, struct msghdr message);

SmolRTSP_Transport smolrtsp_transport_udp(int fd) {
    assert(fd >= 0);

    SmolRTSP_UdpTransport *self = malloc(sizeof *self);
    assert(self);
    self->fd = fd;

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
        .msg_name = NULL,
        .msg_namelen = 0,
        .msg_iov = bufs.ptr,
        .msg_iovlen = bufs.len,
        .msg_control = NULL,
        .msg_controllen = 0,
        .msg_flags = 0,
    };

    return send_packet(self, msg);
}

impl(SmolRTSP_Transport, SmolRTSP_UdpTransport);

static int send_packet(SmolRTSP_UdpTransport *self, struct msghdr message) {
    // Try to retransmit a packet several times on `EMSGSIZE`. The kernel will
    // fragment an IP packet because if `IP_PMTUDISC_WANT` is set.
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
