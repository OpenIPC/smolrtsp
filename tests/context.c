#include <smolrtsp/context.h>

#include <greatest.h>

#include <smolrtsp/types/header.h>

TEST context_creation(void) {
    char buffer[32] = {0};
    const SmolRTSP_Writer w = smolrtsp_string_writer(buffer);

    const uint32_t cseq = 123;

    SmolRTSP_Context *ctx = SmolRTSP_Context_new(w, cseq);

    const SmolRTSP_Writer got_writer = SmolRTSP_Context_get_writer(ctx);
    ASSERT_EQ(w.self, got_writer.self);
    ASSERT_EQ(w.vptr, got_writer.vptr);

    ASSERT_EQ(cseq, SmolRTSP_Context_get_cseq(ctx));
    ASSERT_EQ(0, SmolRTSP_Context_get_ret(ctx));

    VTABLE(SmolRTSP_Context, SmolRTSP_Droppable).drop(ctx);
    PASS();
}

TEST respond_empty(void) {
    char buffer[512] = {0};
    SmolRTSP_Writer w = smolrtsp_string_writer(buffer);
    const uint32_t cseq = 456;

    SmolRTSP_Context *ctx = SmolRTSP_Context_new(w, cseq);

    const char *expected =
        "RTSP/1.0 404 Not found\r\n"
        "CSeq: 456\r\n\r\n";

    ssize_t ret = smolrtsp_respond(ctx, SMOLRTSP_STATUS_NOT_FOUND, "Not found");
    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_EQ(ret, SmolRTSP_Context_get_ret(ctx));
    ASSERT_STR_EQ(expected, buffer);

    VTABLE(SmolRTSP_Context, SmolRTSP_Droppable).drop(ctx);
    PASS();
}

TEST respond(void) {
    char buffer[512] = {0};
    SmolRTSP_Writer w = smolrtsp_string_writer(buffer);
    const uint32_t cseq = 456;

    SmolRTSP_Context *ctx = SmolRTSP_Context_new(w, cseq);

    smolrtsp_header(
        ctx, SMOLRTSP_HEADER_DATE, "%s, 05 Jun 1997 %d:%d:%d GMT", "Thu", 18,
        57, 19);
    smolrtsp_header(
        ctx, SMOLRTSP_HEADER_CONTENT_TYPE, "application/octet-stream");

    smolrtsp_body(ctx, CharSlice99_from_str("1234567890"));

    const char *expected =
        "RTSP/1.0 404 Not found\r\n"
        "CSeq: 456\r\n"
        "Content-Length: 10\r\n"
        "Date: Thu, 05 Jun 1997 18:57:19 GMT\r\n"
        "Content-Type: application/octet-stream\r\n"
        "\r\n1234567890";

    ssize_t ret = smolrtsp_respond(ctx, SMOLRTSP_STATUS_NOT_FOUND, "Not found");
    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_EQ(ret, SmolRTSP_Context_get_ret(ctx));
    ASSERT_STR_EQ(expected, buffer);

    VTABLE(SmolRTSP_Context, SmolRTSP_Droppable).drop(ctx);
    PASS();
}

TEST respond_ok(void) {
    char buffer[512] = {0};
    SmolRTSP_Writer w = smolrtsp_string_writer(buffer);
    const uint32_t cseq = 456;

    SmolRTSP_Context *ctx = SmolRTSP_Context_new(w, cseq);

    const char *expected =
        "RTSP/1.0 200 OK\r\n"
        "CSeq: 456\r\n\r\n";

    ssize_t ret = smolrtsp_respond_ok(ctx);
    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_EQ(ret, SmolRTSP_Context_get_ret(ctx));
    ASSERT_STR_EQ(expected, buffer);

    VTABLE(SmolRTSP_Context, SmolRTSP_Droppable).drop(ctx);
    PASS();
}

TEST respond_internal_error(void) {
    char buffer[512] = {0};
    SmolRTSP_Writer w = smolrtsp_string_writer(buffer);
    const uint32_t cseq = 456;

    SmolRTSP_Context *ctx = SmolRTSP_Context_new(w, cseq);

    const char *expected =
        "RTSP/1.0 500 Internal error\r\n"
        "CSeq: 456\r\n\r\n";

    ssize_t ret = smolrtsp_respond_internal_error(ctx);
    ASSERT_EQ((ssize_t)strlen(expected), ret);
    ASSERT_EQ(ret, SmolRTSP_Context_get_ret(ctx));
    ASSERT_STR_EQ(expected, buffer);

    VTABLE(SmolRTSP_Context, SmolRTSP_Droppable).drop(ctx);
    PASS();
}

SUITE(context) {
    RUN_TEST(context_creation);
    RUN_TEST(respond_empty);
    RUN_TEST(respond);
    RUN_TEST(respond_ok);
    RUN_TEST(respond_internal_error);
}
