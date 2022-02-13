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

declImpl(SmolRTSP_Droppable, SmolRTSP_TcpTransport);
declImpl(SmolRTSP_Transport, SmolRTSP_TcpTransport);

SmolRTSP_Transport smolrtsp_transport_tcp(SmolRTSP_Writer w, uint8_t channel_id) {
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

    const uint32_t interleaved_data =
        SmolRTSP_InterleavedDataHeader_as_u32((SmolRTSP_InterleavedDataHeader){
            .channel_id = self->channel_id,
            .payload_len = htons(total_bytes),
        });

    VCALL(self->w, write, CharSlice99_new((char *)&interleaved_data, sizeof interleaved_data));

    for (size_t i = 0; i < bufs.len; i++) {
        VCALL(self->w, write, CharSlice99_new(bufs.ptr[i].iov_base, bufs.ptr[i].iov_len));
    }

    return 0;
}

impl(SmolRTSP_Transport, SmolRTSP_TcpTransport);
