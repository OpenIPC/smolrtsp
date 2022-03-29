#include <smolrtsp/controller.h>

#include <assert.h>

void smolrtsp_dispatch(
    SmolRTSP_Writer conn, SmolRTSP_Controller controller,
    const SmolRTSP_Request *restrict req) {
    assert(conn.self && conn.vptr);
    assert(controller.self && controller.vptr);
    assert(req);

    SmolRTSP_Context *ctx = SmolRTSP_Context_new(conn, req->cseq);

    if (VCALL(controller, before, ctx, req) == SmolRTSP_ControlFlow_Break) {
        goto after;
    }

    const SmolRTSP_Method method = req->start_line.method,
                          options = SMOLRTSP_METHOD_OPTIONS,
                          describe = SMOLRTSP_METHOD_DESCRIBE,
                          setup = SMOLRTSP_METHOD_SETUP,
                          play = SMOLRTSP_METHOD_PLAY,
                          teardown = SMOLRTSP_METHOD_TEARDOWN;

    if (SmolRTSP_Method_eq(&method, &options)) {
        VCALL(controller, options, ctx, req);
    } else if (SmolRTSP_Method_eq(&method, &describe)) {
        VCALL(controller, describe, ctx, req);
    } else if (SmolRTSP_Method_eq(&method, &setup)) {
        VCALL(controller, setup, ctx, req);
    } else if (SmolRTSP_Method_eq(&method, &play)) {
        VCALL(controller, play, ctx, req);
    } else if (SmolRTSP_Method_eq(&method, &teardown)) {
        VCALL(controller, teardown, ctx, req);
    } else {
        VCALL(controller, unknown, ctx, req);
    }

after:
    VCALL(controller, after, SmolRTSP_Context_get_ret(ctx), ctx, req);

    VTABLE(SmolRTSP_Context, SmolRTSP_Droppable).drop(ctx);
}
