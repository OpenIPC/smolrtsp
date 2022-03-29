#include <smolrtsp/controller.h>

#include <greatest.h>

#include <assert.h>
#include <stdbool.h>

static SmolRTSP_Request options_req, describe_req, setup_req, play_req,
    teardown_req, abracadabra_req;

typedef struct {
    bool options_completed, describe_completed, setup_completed, play_completed,
        teardown_completed, abracadabra_completed;
    int before_invoked_n, after_invoked_n;
    bool dropped;
} Client;

static void Client_drop(VSelf) {
    VSELF(Client);
    self->dropped = true;
}

impl(SmolRTSP_Droppable, Client);

#define HEADER_TEST_METHOD CharSlice99_from_str("Test-Method")

static void
Client_options(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    self->options_completed = true;
    assert(req == &options_req);
    smolrtsp_header(ctx, HEADER_TEST_METHOD, "options");
    const ssize_t ret = smolrtsp_respond_ok(ctx);
    assert(ret > 0);
}

static void
Client_describe(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    self->describe_completed = true;
    assert(req == &describe_req);
    smolrtsp_header(ctx, HEADER_TEST_METHOD, "describe");
    const ssize_t ret = smolrtsp_respond_ok(ctx);
    assert(ret > 0);
}

static void
Client_setup(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    self->setup_completed = true;
    assert(req == &setup_req);
    smolrtsp_header(ctx, HEADER_TEST_METHOD, "setup");
    const ssize_t ret = smolrtsp_respond_ok(ctx);
    assert(ret > 0);
}

static void
Client_play(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    self->play_completed = true;
    assert(req == &play_req);
    smolrtsp_header(ctx, HEADER_TEST_METHOD, "play");
    const ssize_t ret = smolrtsp_respond_ok(ctx);
    assert(ret > 0);
}

static void
Client_teardown(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    self->teardown_completed = true;
    assert(req == &teardown_req);
    smolrtsp_header(ctx, HEADER_TEST_METHOD, "teardown");
    const ssize_t ret = smolrtsp_respond_ok(ctx);
    assert(ret > 0);
}

static void
Client_unknown(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    self->abracadabra_completed = true;
    assert(req == &abracadabra_req);
    smolrtsp_header(ctx, HEADER_TEST_METHOD, "abracadabra");
    const ssize_t ret = smolrtsp_respond_ok(ctx);
    assert(ret > 0);
}

static SmolRTSP_ControlFlow
Client_before(VSelf, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);
    self->before_invoked_n++;

    (void)req;

    // Set up initial headers.
    smolrtsp_header(ctx, SMOLRTSP_HEADER_AUTHORIZATION, "WWW-Authenticate");
    smolrtsp_header(ctx, SMOLRTSP_HEADER_SERVER, "MyServer");

    return SmolRTSP_ControlFlow_Continue;
}

static void Client_after(
    VSelf, ssize_t ret, SmolRTSP_Context *ctx, const SmolRTSP_Request *req) {
    VSELF(Client);

    (void)ctx;
    (void)req;

    self->after_invoked_n++;

    assert(ret > 0);
}

impl(SmolRTSP_Controller, Client);

TEST dispatch(void) {
    options_req.cseq = describe_req.cseq = setup_req.cseq = play_req.cseq =
        teardown_req.cseq = abracadabra_req.cseq = 123;

    options_req.start_line.method = SMOLRTSP_METHOD_OPTIONS;
    describe_req.start_line.method = SMOLRTSP_METHOD_DESCRIBE;
    setup_req.start_line.method = SMOLRTSP_METHOD_SETUP;
    play_req.start_line.method = SMOLRTSP_METHOD_PLAY;
    teardown_req.start_line.method = SMOLRTSP_METHOD_TEARDOWN;
    abracadabra_req.start_line.method = CharSlice99_from_str("Abracadabra");

    char buffer[256] = {0};
    const SmolRTSP_Writer sdp = smolrtsp_string_writer(buffer);

    Client client = {
        .options_completed = false,
        .describe_completed = false,
        .setup_completed = false,
        .play_completed = false,
        .teardown_completed = false,
        .abracadabra_completed = false,
        .before_invoked_n = 0,
        .after_invoked_n = 0,
        .dropped = false,
    };

    SmolRTSP_Controller controller = DYN(Client, SmolRTSP_Controller, &client);

    int before_after_invoked_n = 0;

#define CHECK(method)                                                          \
    do {                                                                       \
        smolrtsp_dispatch(sdp, controller, &method##_req);                     \
        before_after_invoked_n++;                                              \
        ASSERT(client.method##_completed);                                     \
        ASSERT_EQ(before_after_invoked_n, client.before_invoked_n);            \
        ASSERT_EQ(before_after_invoked_n, client.after_invoked_n);             \
        ASSERT_STR_EQ(                                                         \
            "RTSP/1.0 200 OK\r\n"                                              \
            "CSeq: 123\r\n"                                                    \
            "Authorization: WWW-Authenticate\r\n"                              \
            "Server: MyServer\r\n"                                             \
            "Test-Method: " #method "\r\n\r\n",                                \
            buffer);                                                           \
        buffer[0] = '\0';                                                      \
    } while (0)

    CHECK(options);
    CHECK(describe);
    CHECK(setup);
    CHECK(play);
    CHECK(teardown);
    CHECK(abracadabra);

#undef CHECK

    PASS();
}

SUITE(controller) {
    RUN_TEST(dispatch);
}
