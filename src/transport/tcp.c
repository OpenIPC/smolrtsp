#include <smolrtsp/transport.h>

#include <alloca.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#include <slice99.h>

#include <smolrtsp/util.h>

typedef struct {
    SmolRTSP_Writer w;
    int channel_id;
    size_t max_buffer;
} SmolRTSP_TcpTransport;

declImpl(SmolRTSP_Transport, SmolRTSP_TcpTransport);

SmolRTSP_Transport smolrtsp_transport_tcp(
    SmolRTSP_Writer w, uint8_t channel_id, size_t max_buffer) {
    assert(w.self && w.vptr);

    SmolRTSP_TcpTransport *self = malloc(sizeof *self);
    assert(self);

    self->w = w;
    self->channel_id = channel_id;
    self->max_buffer = max_buffer;

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

    /* Build the full interleaved RTSP frame (4-byte header + payload
     * iovec) in a single contiguous buffer, then issue exactly ONE
     * write to the underlying Writer. Splitting the frame into
     * header + per-iovec writes interacts badly with downstream
     * Writers backed by libevent's bufferevent — the multi-step
     * sequence of evbuffer_add calls can desync the chain's
     * last_with_datap / free-space bookkeeping under certain
     * fragment-size patterns, producing a heap-buffer-overflow at
     * exactly one chunk boundary past the end of the current
     * evbuffer chain. Coalescing into one write keeps every
     * interleaved frame inside a single evbuffer_add and side-steps
     * the multi-step interaction entirely. It also matches the UDP
     * transport's behaviour (one sendmsg per RTP packet) and tightens
     * the critical section around the Writer lock. */
    const size_t frame_len = sizeof header + total_bytes;
    uint8_t *frame = alloca(frame_len);
    memcpy(frame, &header, sizeof header);
    size_t off = sizeof header;
    for (size_t i = 0; i < bufs.len; i++) {
        memcpy(frame + off, bufs.ptr[i].iov_base, bufs.ptr[i].iov_len);
        off += bufs.ptr[i].iov_len;
    }
    assert(off == frame_len);

    VCALL(self->w, lock);
    const ssize_t ret =
        VCALL(self->w, write, CharSlice99_new((char *)frame, frame_len));
    VCALL(self->w, unlock);

    return ret == (ssize_t)frame_len ? 0 : -1;
}

static bool SmolRTSP_TcpTransport_is_full(VSelf) {
    VSELF(SmolRTSP_TcpTransport);
    assert(self);

    return VCALL(self->w, filled) > self->max_buffer;
}

impl(SmolRTSP_Transport, SmolRTSP_TcpTransport);
