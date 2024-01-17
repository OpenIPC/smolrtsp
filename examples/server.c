/*
 * A simple RTSP server implementation using libevent [1].
 *
 * To obtain `audio.g711a` and `video.h264`:
 *
 * $ ffmpeg -i http://docs.evostream.com/sample_content/assets/bun33s.mp4 \
 *     -acodec pcm_mulaw -f mulaw -ar 8000 -ac 1 audio.g711a \
 *     -vcodec h264 -x264opts aud=1 video.h264
 *
 * [1] https://libevent.org/
 */

#include <smolrtsp.h>

#include <smolrtsp-libevent.h>

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/util.h>

// G.711 A-Law, 8k sample rate, mono channel.
#include "media/audio.g711a.h"

// H.264 video with AUDs, 25 FPS.
#include "media/video.h264.h"

// Comment one of these lines if you do not need audio or video.
#define ENABLE_AUDIO
#define ENABLE_VIDEO

#define SERVER_PORT SMOLRTSP_DEFAULT_PORT

#define AUDIO_PCMU_PAYLOAD_TYPE  0
#define AUDIO_SAMPLE_RATE        8000
#define AUDIO_SAMPLES_PER_PACKET 160
#define AUDIO_PACKETIZATION_TIME_US                                            \
    (1e6 / (AUDIO_SAMPLE_RATE / AUDIO_SAMPLES_PER_PACKET))

#define VIDEO_PAYLOAD_TYPE 96 // dynamic PT
#define VIDEO_SAMPLE_RATE  90000
#define VIDEO_FPS          25

#define AUDIO_STREAM_ID 0
#define VIDEO_STREAM_ID 1

#define MAX_STREAMS 2

typedef struct {
    uint64_t session_id;
    SmolRTSP_RtpTransport *transport;
    struct event *ev;
    SmolRTSP_Droppable ctx;
} Stream;

typedef struct {
    struct event_base *base;
    struct bufferevent *bev;
    struct sockaddr_storage addr;
    size_t addr_len;
    Stream streams[MAX_STREAMS];
    int streams_playing;
} Client;

declImpl(SmolRTSP_Controller, Client);

static void listener_cb(
    struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa,
    int socklen, void *ctx);
static void on_event_cb(struct bufferevent *bev, short events, void *ctx);
static void on_sigint_cb(evutil_socket_t sig, short events, void *ctx);

static int setup_transport(
    Client *self, SmolRTSP_Context *ctx, const SmolRTSP_Request *req,
    SmolRTSP_Transport *t);
static int setup_tcp(
    SmolRTSP_Context *ctx, SmolRTSP_Transport *t,
    SmolRTSP_TransportConfig config);
static int setup_udp(
    const struct sockaddr *addr, SmolRTSP_Context *ctx, SmolRTSP_Transport *t,
    SmolRTSP_TransportConfig config);

typedef struct {
    SmolRTSP_RtpTransport *transport;
    size_t i;
    struct event *ev;
    struct bufferevent *bev;
    int *streams_playing;
} AudioCtx;

static SmolRTSP_Droppable play_audio(
    struct event_base *base, struct bufferevent *bev, SmolRTSP_RtpTransport *t,
    struct event **ev, int *streams_playing);
static void send_audio_packet_cb(evutil_socket_t fd, short events, void *arg);

typedef struct {
    SmolRTSP_NalTransport *transport;
    SmolRTSP_NalStartCodeTester start_code_tester;
    uint32_t timestamp;
    U8Slice99 video;
    uint8_t *nalu_start;
    struct event *ev;
    struct bufferevent *bev;
    int *streams_playing;
} VideoCtx;

static SmolRTSP_Droppable play_video(
    struct event_base *base, struct bufferevent *bev, SmolRTSP_RtpTransport *t,
    struct event **ev, int *streams_playing);
static void send_video_packet_cb(evutil_socket_t fd, short events, void *arg);
static bool send_nalu(VideoCtx *ctx);

int main(void) {
    srand(time(NULL));

    struct event_base *base;
    if ((base = event_base_new()) == NULL) {
        fputs("event_base_new failed.\n", stderr);
        return EXIT_FAILURE;
    }

    struct sockaddr_in sin = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
    };

    struct evconnlistener *listener;
    if ((listener = evconnlistener_new_bind(
             base, listener_cb, (void *)base,
             LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
             (struct sockaddr *)&sin, sizeof sin)) == NULL) {
        fputs("evconnlistener_new_bind failed.\n", stderr);
        return EXIT_FAILURE;
    }

    struct event *sigint_handler;
    if ((sigint_handler =
             evsignal_new(base, SIGINT, on_sigint_cb, (void *)base)) == NULL) {
        fputs("evsignal_new failed.\n", stderr);
        return EXIT_FAILURE;
    }

    if (event_add(sigint_handler, NULL) < 0) {
        fputs("event_add failed.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("Server started on port %d.\n", SERVER_PORT);

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_free(sigint_handler);
    event_base_free(base);

    puts("Done.");
    return EXIT_SUCCESS;
}

static void listener_cb(
    struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa,
    int socklen, void *arg) {
    (void)listener;
    (void)fd;
    (void)socklen;

    struct event_base *base = arg;

    struct bufferevent *bev;
    if ((bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE)) ==
        NULL) {
        fputs("bufferevent_socket_new failed.\n", stderr);
        event_base_loopbreak(base);
        return;
    }

    Client *client = calloc(1, sizeof *client);
    assert(client);
    client->base = base;
    client->bev = bev;
    memcpy(&client->addr, sa, socklen);
    client->addr_len = socklen;

    SmolRTSP_Controller controller = DYN(Client, SmolRTSP_Controller, client);
    void *ctx = smolrtsp_libevent_ctx(controller);

    bufferevent_setcb(bev, smolrtsp_libevent_cb, NULL, on_event_cb, ctx);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

static void on_event_cb(struct bufferevent *bev, short events, void *ctx) {
    if (events & BEV_EVENT_EOF) {
        puts("Connection closed.");
    } else if (events & BEV_EVENT_ERROR) {
        perror("Got an error on the connection");
    }

    bufferevent_free(bev);
    smolrtsp_libevent_ctx_free(ctx);
}

static void on_sigint_cb(evutil_socket_t sig, short events, void *ctx) {
    (void)sig;
    (void)events;

    struct event_base *base = ctx;

    puts("Caught an interrupt signal; exiting cleanly in two seconds.");

    struct timeval delay = {2, 0};
    event_base_loopexit(base, &delay);
}

static void Client_drop(VSelf) {
    VSELF(Client);

    for (size_t i = 0; i < MAX_STREAMS; i++) {
        if (self->streams[i].ctx.vptr != NULL) {
            VCALL(self->streams[i].ctx, drop);
        }
    }

    free(self);
}

impl(SmolRTSP_Droppable, Client);

static void
Client_options(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    (void)self;
    (void)req;

    smolrtsp_header(
        ctx, SMOLRTSP_HEADER_PUBLIC, "DESCRIBE, SETUP, TEARDOWN, PLAY");
    smolrtsp_respond_ok(ctx);
}

static void
Client_describe(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    (void)self;
    (void)req;

    char sdp_buf[1024] = {0};
    SmolRTSP_Writer sdp = smolrtsp_string_writer(sdp_buf);
    ssize_t ret = 0;

    // clang-format off
    SMOLRTSP_SDP_DESCRIBE(
        ret, sdp,
        (SMOLRTSP_SDP_VERSION, "0"),
        (SMOLRTSP_SDP_ORIGIN, "SmolRTSP 3855320066 3855320129 IN IP4 0.0.0.0"),
        (SMOLRTSP_SDP_SESSION_NAME, "SmolRTSP example"),
        (SMOLRTSP_SDP_CONNECTION, "IN IP4 0.0.0.0"),
        (SMOLRTSP_SDP_TIME, "0 0"));

#ifdef ENABLE_AUDIO
    SMOLRTSP_SDP_DESCRIBE(
        ret, sdp,
        (SMOLRTSP_SDP_MEDIA, "audio 0 RTP/AVP %d", AUDIO_PCMU_PAYLOAD_TYPE),
        (SMOLRTSP_SDP_ATTR, "control:audio"));
#endif

#ifdef ENABLE_VIDEO
    SMOLRTSP_SDP_DESCRIBE(
        ret, sdp,
        (SMOLRTSP_SDP_MEDIA, "video 0 RTP/AVP %d", VIDEO_PAYLOAD_TYPE),
        (SMOLRTSP_SDP_ATTR, "control:video"),
        (SMOLRTSP_SDP_ATTR, "rtpmap:%d H264/%" PRIu32, VIDEO_PAYLOAD_TYPE, VIDEO_SAMPLE_RATE),
        (SMOLRTSP_SDP_ATTR, "fmtp:%d packetization-mode=1", VIDEO_PAYLOAD_TYPE),
        (SMOLRTSP_SDP_ATTR, "framerate:%d", VIDEO_FPS));
#endif
    // clang-format on

    assert(ret > 0);

    smolrtsp_header(ctx, SMOLRTSP_HEADER_CONTENT_TYPE, "application/sdp");
    smolrtsp_body(ctx, CharSlice99_from_str(sdp_buf));

    smolrtsp_respond_ok(ctx);
}

static void
Client_setup(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    SmolRTSP_Transport transport;
    if (setup_transport(self, ctx, req, &transport) == -1) {
        return;
    }

    const size_t stream_id =
        CharSlice99_primitive_ends_with(
            req->start_line.uri, CharSlice99_from_str("/audio"))
            ? AUDIO_STREAM_ID
            : VIDEO_STREAM_ID;
    Stream *stream = &self->streams[stream_id];

    const bool aggregate_control_requested = SmolRTSP_HeaderMap_contains_key(
        &req->header_map, SMOLRTSP_HEADER_SESSION);
    if (aggregate_control_requested) {
        uint64_t session_id;
        if (smolrtsp_scanf_header(
                &req->header_map, SMOLRTSP_HEADER_SESSION, "%" SCNu64,
                &session_id) != 1) {
            smolrtsp_respond(
                ctx, SMOLRTSP_STATUS_BAD_REQUEST, "Malformed `Session'");
            return;
        }

        stream->session_id = session_id;
    } else {
        stream->session_id = (uint64_t)rand();
    }

    if (AUDIO_STREAM_ID == stream_id) {
        stream->transport = SmolRTSP_RtpTransport_new(
            transport, AUDIO_PCMU_PAYLOAD_TYPE, AUDIO_SAMPLE_RATE);
    } else {
        stream->transport = SmolRTSP_RtpTransport_new(
            transport, VIDEO_PAYLOAD_TYPE, VIDEO_SAMPLE_RATE);
    }

    smolrtsp_header(
        ctx, SMOLRTSP_HEADER_SESSION, "%" PRIu64, stream->session_id);

    smolrtsp_respond_ok(ctx);
}

static void
Client_play(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    uint64_t session_id;
    if (smolrtsp_scanf_header(
            &req->header_map, SMOLRTSP_HEADER_SESSION, "%" SCNu64,
            &session_id) != 1) {
        smolrtsp_respond(
            ctx, SMOLRTSP_STATUS_BAD_REQUEST, "Malformed `Session'");
        return;
    }

    bool played = false;
    for (size_t i = 0; i < MAX_STREAMS; i++) {
        if (self->streams[i].session_id == session_id) {
            if (AUDIO_STREAM_ID == i) {
                self->streams[i].ctx = play_audio(
                    self->base, self->bev, self->streams[i].transport,
                    &self->streams[i].ev, &self->streams_playing);
            } else {
                self->streams[i].ctx = play_video(
                    self->base, self->bev, self->streams[i].transport,
                    &self->streams[i].ev, &self->streams_playing);
            }

            played = true;
        }
    }

    if (!played) {
        smolrtsp_respond(
            ctx, SMOLRTSP_STATUS_SESSION_NOT_FOUND, "Invalid Session ID");
        return;
    }

    smolrtsp_header(ctx, SMOLRTSP_HEADER_RANGE, "npt=now-");
    smolrtsp_respond_ok(ctx);
}

static void
Client_teardown(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    uint64_t session_id;
    if (smolrtsp_scanf_header(
            &req->header_map, SMOLRTSP_HEADER_SESSION, "%" SCNu64,
            &session_id) != 1) {
        smolrtsp_respond(
            ctx, SMOLRTSP_STATUS_BAD_REQUEST, "Malformed `Session'");
        return;
    }

    bool teardowned = false;
    for (size_t i = 0; i < MAX_STREAMS; i++) {
        if (self->streams[i].session_id == session_id) {
            event_del(self->streams[i].ev);
            teardowned = true;
        }
    }

    if (!teardowned) {
        smolrtsp_respond(
            ctx, SMOLRTSP_STATUS_SESSION_NOT_FOUND, "Invalid Session ID");
        return;
    }

    smolrtsp_respond_ok(ctx);
}

static void
Client_unknown(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    (void)self;
    (void)req;

    smolrtsp_respond(ctx, SMOLRTSP_STATUS_METHOD_NOT_ALLOWED, "Unknown method");
}

static SmolRTSP_ControlFlow
Client_before(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    (void)self;
    (void)ctx;

    printf(
        "%s %s CSeq=%" PRIu32 ".\n",
        CharSlice99_alloca_c_str(req->start_line.method),
        CharSlice99_alloca_c_str(req->start_line.uri), req->cseq);

    return SmolRTSP_ControlFlow_Continue;
}

static void Client_after(
    VSelf, ssize_t ret, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    (void)self;
    (void)ctx;
    (void)req;

    if (ret < 0) {
        perror("Failed to respond");
    }
}

impl(SmolRTSP_Controller, Client);

static int setup_transport(
    Client *self, SmolRTSP_Context *ctx, const SmolRTSP_Request *req,
    SmolRTSP_Transport *t) {
    CharSlice99 transport_val;
    const bool transport_found = SmolRTSP_HeaderMap_find(
        &req->header_map, SMOLRTSP_HEADER_TRANSPORT, &transport_val);
    if (!transport_found) {
        smolrtsp_respond(
            ctx, SMOLRTSP_STATUS_BAD_REQUEST, "`Transport' not present");
        return -1;
    }

    SmolRTSP_TransportConfig config;
    if (smolrtsp_parse_transport(&config, transport_val) == -1) {
        smolrtsp_respond(
            ctx, SMOLRTSP_STATUS_BAD_REQUEST, "Malformed `Transport'");
        return -1;
    }

    switch (config.lower) {
    case SmolRTSP_LowerTransport_TCP:
        if (setup_tcp(ctx, t, config) == -1) {
            smolrtsp_respond_internal_error(ctx);
            return -1;
        }
        break;
    case SmolRTSP_LowerTransport_UDP:
        if (setup_udp((const struct sockaddr *)&self->addr, ctx, t, config) ==
            -1) {
            smolrtsp_respond_internal_error(ctx);
            return -1;
        }
        break;
    }

    return 0;
}

static int setup_tcp(
    SmolRTSP_Context *ctx, SmolRTSP_Transport *t,
    SmolRTSP_TransportConfig config) {
    ifLet(config.interleaved, SmolRTSP_ChannelPair_Some, interleaved) {
        *t = smolrtsp_transport_tcp(
            SmolRTSP_Context_get_writer(ctx), interleaved->rtp_channel, 0);

        smolrtsp_header(
            ctx, SMOLRTSP_HEADER_TRANSPORT,
            "RTP/AVP/TCP;unicast;interleaved=%" PRIu8 "-%" PRIu8,
            interleaved->rtp_channel, interleaved->rtcp_channel);
        return 0;
    }

    smolrtsp_respond(
        ctx, SMOLRTSP_STATUS_BAD_REQUEST, "`interleaved' not found");
    return -1;
}

static int setup_udp(
    const struct sockaddr *addr, SmolRTSP_Context *ctx, SmolRTSP_Transport *t,
    SmolRTSP_TransportConfig config) {

    ifLet(config.client_port, SmolRTSP_PortPair_Some, client_port) {
        int fd;
        if ((fd = smolrtsp_dgram_socket(
                 addr->sa_family, smolrtsp_sockaddr_ip(addr),
                 client_port->rtp_port)) == -1) {
            return -1;
        }

        *t = smolrtsp_transport_udp(fd);

        smolrtsp_header(
            ctx, SMOLRTSP_HEADER_TRANSPORT,
            "RTP/AVP/UDP;unicast;client_port=%" PRIu16 "-%" PRIu16,
            client_port->rtp_port, client_port->rtcp_port);
        return 0;
    }

    smolrtsp_respond(
        ctx, SMOLRTSP_STATUS_BAD_REQUEST, "`client_port' not found");
    return -1;
}

static void AudioCtx_drop(VSelf) {
    VSELF(AudioCtx);

    event_free(self->ev);
    VTABLE(SmolRTSP_RtpTransport, SmolRTSP_Droppable).drop(self->transport);
    free(self);
}

impl(SmolRTSP_Droppable, AudioCtx);

static SmolRTSP_Droppable play_audio(
    struct event_base *base, struct bufferevent *bev, SmolRTSP_RtpTransport *t,
    struct event **ev, int *streams_playing) {
    AudioCtx *ctx = malloc(sizeof *ctx);
    assert(ctx);
    *ctx = (AudioCtx){
        .transport = t,
        .i = 0,
        .ev = NULL,
        .streams_playing = streams_playing,
        .bev = bev,
    };

    ctx->ev = event_new(
        base, -1, EV_PERSIST | EV_TIMEOUT, send_audio_packet_cb, (void *)ctx);
    assert(ctx->ev);

    event_add(
        ctx->ev, &(const struct timeval){
                     .tv_sec = 0,
                     .tv_usec = AUDIO_PACKETIZATION_TIME_US,
                 });
    *ev = ctx->ev;
    (*streams_playing)++;

    return DYN(AudioCtx, SmolRTSP_Droppable, ctx);
}

static void send_audio_packet_cb(evutil_socket_t fd, short events, void *arg) {
    (void)fd;
    (void)events;

    AudioCtx *ctx = arg;

    if (ctx->i * AUDIO_SAMPLES_PER_PACKET >= ___media_audio_g711a_len) {
        event_del(ctx->ev);
        (*ctx->streams_playing)--;
        if (0 == *ctx->streams_playing) {
            bufferevent_trigger_event(ctx->bev, BEV_EVENT_EOF, 0);
        }
        return;
    }

    const SmolRTSP_RtpTimestamp ts =
        SmolRTSP_RtpTimestamp_Raw(ctx->i * AUDIO_SAMPLES_PER_PACKET);
    const bool marker = false;
    const size_t samples_count =
        ___media_audio_g711a_len <
                ctx->i * AUDIO_SAMPLES_PER_PACKET + AUDIO_SAMPLES_PER_PACKET
            ? ___media_audio_g711a_len % AUDIO_SAMPLES_PER_PACKET
            : AUDIO_SAMPLES_PER_PACKET;
    const U8Slice99 header = U8Slice99_empty(),
                    payload = U8Slice99_new(
                        ___media_audio_g711a +
                            ctx->i * AUDIO_SAMPLES_PER_PACKET,
                        samples_count);

    if (SmolRTSP_RtpTransport_send_packet(
            ctx->transport, ts, marker, header, payload) == -1) {
        perror("Failed to send RTP/PCMU");
    }

    ctx->i++;
}

static void VideoCtx_drop(VSelf) {
    VSELF(VideoCtx);

    event_free(self->ev);
    VTABLE(SmolRTSP_NalTransport, SmolRTSP_Droppable).drop(self->transport);
    free(self);
}

impl(SmolRTSP_Droppable, VideoCtx);

static SmolRTSP_Droppable play_video(
    struct event_base *base, struct bufferevent *bev, SmolRTSP_RtpTransport *t,
    struct event **ev, int *streams_playing) {
    U8Slice99 video = Slice99_typed_from_array(___media_video_h264);

    SmolRTSP_NalStartCodeTester start_code_tester;
    if ((start_code_tester = smolrtsp_determine_start_code(video)) == NULL) {
        fputs("Invalid video file.\n", stderr);
        abort();
    }

    VideoCtx *ctx = malloc(sizeof *ctx);
    assert(ctx);
    *ctx = (VideoCtx){
        .transport = SmolRTSP_NalTransport_new(t),
        .start_code_tester = start_code_tester,
        .timestamp = 0,
        .video = video,
        .nalu_start = NULL,
        .ev = NULL,
        .bev = bev,
        .streams_playing = streams_playing,
    };

    ctx->ev = event_new(
        base, -1, EV_PERSIST | EV_TIMEOUT, send_video_packet_cb, (void *)ctx);
    assert(ctx->ev);

    event_add(
        ctx->ev, &(const struct timeval){
                     .tv_sec = 0,
                     .tv_usec = 1e6 / VIDEO_FPS,
                 });
    *ev = ctx->ev;
    (*streams_playing)++;

    return DYN(VideoCtx, SmolRTSP_Droppable, ctx);
}

static void send_video_packet_cb(evutil_socket_t fd, short events, void *arg) {
    (void)fd;
    (void)events;

    VideoCtx *ctx = arg;

again:
    if (U8Slice99_is_empty(ctx->video)) {
        send_nalu(ctx);
        event_del(ctx->ev);
        (*ctx->streams_playing)--;
        if (0 == *ctx->streams_playing) {
            bufferevent_trigger_event(ctx->bev, BEV_EVENT_EOF, 0);
        }
        return;
    }

    const size_t start_code_len = ctx->start_code_tester(ctx->video);
    if (0 == start_code_len) {
        ctx->video = U8Slice99_advance(ctx->video, 1);
        goto again;
    }

    bool au_found = false;
    if (NULL != ctx->nalu_start) {
        au_found = send_nalu(ctx);
    }

    ctx->video = U8Slice99_advance(ctx->video, start_code_len);
    ctx->nalu_start = ctx->video.ptr;

    if (!au_found) {
        goto again;
    }
}

static bool send_nalu(VideoCtx *ctx) {
    const SmolRTSP_NalUnit nalu = {
        .header = SmolRTSP_NalHeader_H264(
            SmolRTSP_H264NalHeader_parse(ctx->nalu_start[0])),
        .payload = U8Slice99_from_ptrdiff(ctx->nalu_start + 1, ctx->video.ptr),
    };

    bool au_found = false;

    if (SmolRTSP_NalHeader_unit_type(nalu.header) ==
        SMOLRTSP_H264_NAL_UNIT_AUD) {
        ctx->timestamp += VIDEO_SAMPLE_RATE / VIDEO_FPS;
        au_found = true;
    }

    if (SmolRTSP_NalTransport_send_packet(
            ctx->transport, SmolRTSP_RtpTimestamp_Raw(ctx->timestamp), nalu) ==
        -1) {
        perror("Failed to send RTP/NAL");
    }

    return au_found;
}
