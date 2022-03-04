#include <smolrtsp/context.h>

#include <smolrtsp/types/header_map.h>
#include <smolrtsp/types/response.h>

#include <assert.h>
#include <stdlib.h>

struct SmolRTSP_Context {
    SmolRTSP_Writer writer;
    uint32_t cseq;
    SmolRTSP_HeaderMap headers;
    SmolRTSP_MessageBody body;
};

SmolRTSP_Context *SmolRTSP_Context_new(SmolRTSP_Writer w, uint32_t cseq) {
    assert(w.self && w.vptr);

    SmolRTSP_Context *self = malloc(sizeof *self);
    assert(self);
    self->writer = w;
    self->cseq = cseq;
    self->headers = SmolRTSP_HeaderMap_empty();
    self->body = SmolRTSP_MessageBody_empty();

    return self;
}

SmolRTSP_Writer SmolRTSP_Context_get_writer(const SmolRTSP_Context *ctx) {
    assert(ctx);
    return ctx->writer;
}

uint32_t SmolRTSP_Context_get_cseq(const SmolRTSP_Context *ctx) {
    assert(ctx);
    return ctx->cseq;
}

void smolrtsp_header(
    SmolRTSP_Context *ctx, CharSlice99 key, CharSlice99 value) {
    assert(ctx);
    SmolRTSP_HeaderMap_append(&ctx->headers, (SmolRTSP_Header){key, value});
}

void smolrtsp_body(SmolRTSP_Context *ctx, SmolRTSP_MessageBody body) {
    assert(ctx);
    ctx->body = body;
}

ssize_t smolrtsp_respond(
    SmolRTSP_Context *ctx, SmolRTSP_StatusCode code, const char *reason) {
    assert(ctx);
    assert(reason);

    const SmolRTSP_Response response = {
        .start_line =
            {
                .version = {.major = 1, .minor = 0},
                .code = code,
                .reason = CharSlice99_from_str((char *)reason),
            },
        .header_map = ctx->headers,
        .body = ctx->body,
        .cseq = ctx->cseq,
    };

    return SmolRTSP_Response_serialize(response, ctx->writer);
}

void SmolRTSP_Context_drop(VSelf) {
    VSELF(SmolRTSP_Context);
    assert(self);

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_Context);
