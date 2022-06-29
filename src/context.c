#include <smolrtsp/context.h>

#include <smolrtsp/types/header_map.h>
#include <smolrtsp/types/response.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct SmolRTSP_Context {
    SmolRTSP_Writer writer;
    uint32_t cseq;
    SmolRTSP_HeaderMap header_map;
    SmolRTSP_MessageBody body;
    ssize_t ret;
};

SmolRTSP_Context *SmolRTSP_Context_new(SmolRTSP_Writer w, uint32_t cseq) {
    assert(w.self && w.vptr);

    SmolRTSP_Context *self = malloc(sizeof *self);
    assert(self);
    self->writer = w;
    self->cseq = cseq;
    self->header_map = SmolRTSP_HeaderMap_empty();
    self->body = SmolRTSP_MessageBody_empty();
    self->ret = 0;

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

ssize_t SmolRTSP_Context_get_ret(const SmolRTSP_Context *ctx) {
    assert(ctx);
    return ctx->ret;
}

void smolrtsp_vheader(
    SmolRTSP_Context *ctx, CharSlice99 key, const char *restrict fmt,
    va_list list) {
    assert(ctx);
    assert(fmt);

    va_list list_copy;
    va_copy(list_copy, list);

    const int space_required = vsnprintf(NULL, 0, fmt, list_copy);
    assert(space_required > 0);
    char *value = malloc(space_required + 1 /* null character */);
    assert(value);

    const int bytes_written __attribute__((unused)) =
        vsprintf(value, fmt, list);
    assert(space_required == bytes_written);

    const SmolRTSP_Header h = {key, CharSlice99_from_str(value)};
    SmolRTSP_HeaderMap_append(&ctx->header_map, h);
}

void smolrtsp_header(
    SmolRTSP_Context *ctx, CharSlice99 key, const char *restrict fmt, ...) {
    assert(ctx);
    assert(fmt);

    va_list ap;
    va_start(ap, fmt);
    smolrtsp_vheader(ctx, key, fmt, ap);
    va_end(ap);
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
        .header_map = ctx->header_map,
        .body = ctx->body,
        .cseq = ctx->cseq,
    };

    ctx->ret = SmolRTSP_Response_serialize(&response, ctx->writer);
    return ctx->ret;
}

ssize_t smolrtsp_respond_ok(SmolRTSP_Context *ctx) {
    assert(ctx);
    return smolrtsp_respond(ctx, SMOLRTSP_STATUS_OK, "OK");
}

ssize_t smolrtsp_respond_internal_error(SmolRTSP_Context *ctx) {
    assert(ctx);
    return smolrtsp_respond(
        ctx, SMOLRTSP_STATUS_INTERNAL_SERVER_ERROR, "Internal error");
}

void SmolRTSP_Context_drop(VSelf) {
    VSELF(SmolRTSP_Context);
    assert(self);

    for (size_t i = 0; i < self->header_map.len; i++) {
        free(self->header_map.headers[i].value.ptr);
    }

    free(self);
}

implExtern(SmolRTSP_Droppable, SmolRTSP_Context);
