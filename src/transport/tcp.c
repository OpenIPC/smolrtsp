#include <smolrtsp/transport.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <arpa/inet.h>

#include <slice99.h>

#include <smolrtsp/util.h>

typedef struct {
    SmolRTSP_Writer w;
    int channel_id;
} SmolRTSP_TcpTransport;

declImpl(SmolRTSP_Transport, SmolRTSP_TcpTransport);

SmolRTSP_Transport
smolrtsp_transport_tcp(SmolRTSP_Writer w, uint8_t channel_id) {
    assert(w.self && w.vptr);

    SmolRTSP_TcpTransport *self = malloc(sizeof *self);
    assert(self);

    self->w = w;
    self->channel_id = channel_id;

    return DYN(SmolRTSP_TcpTransport, SmolRTSP_Transport, self);
}

static void SmolRTSP_TcpTransport_drop(VSelf) {
    VSELF(SmolRTSP_TcpTransport);
    assert(self);

    free(self);
}

impl(SmolRTSP_Droppable, SmolRTSP_TcpTransport);

static int SmolRTSP_TcpTransport_transmit(VSelf, SmolRTSP_IoVecSlice bufs) {
    VSELF(SmolRTSP_TcpTransport);
    assert(self);

    const size_t total_bytes = SmolRTSP_IoVecSlice_len(bufs);

    const uint32_t header =
        smolrtsp_interleaved_header(self->channel_id, htons(total_bytes));

    ssize_t ret =
        VCALL(self->w, write, CharSlice99_new((char *)&header, sizeof header));
    if (ret != sizeof header) {
        return -1;
    }

    for (size_t i = 0; i < bufs.len; i++) {
        const CharSlice99 vec =
            CharSlice99_new(bufs.ptr[i].iov_base, bufs.ptr[i].iov_len);
        ret = VCALL(self->w, write, vec);
        if (ret != (ssize_t)vec.len) {
            return -1;
        }
    }

    return 0;
}

impl(SmolRTSP_Transport, SmolRTSP_TcpTransport);
