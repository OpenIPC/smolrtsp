#include <smolrtsp/controller.h>

#include <assert.h>

void smolrtsp_dispatch(
    SmolRTSP_Writer conn, SmolRTSP_Controller controller,
    const SmolRTSP_Request *restrict req) {
    assert(conn.self && conn.vptr);
    assert(controller.self && controller.vptr);
    assert(req);

    SmolRTSP_Context *ctx = SmolRTSP_Context_new(conn, req->cseq);

    VCALL(controller, before, ctx, req);

    const SmolRTSP_Method method = req->start_line.method,
                          options = SMOLRTSP_METHOD_OPTIONS,
                          describe = SMOLRTSP_METHOD_DESCRIBE,
                          setup = SMOLRTSP_METHOD_SETUP,
                          play = SMOLRTSP_METHOD_PLAY,
                          teardown = SMOLRTSP_METHOD_TEARDOWN;

    ssize_t ret;

    if (SmolRTSP_Method_eq(&method, &options)) {
        ret = VCALL(controller, options, ctx, req);
    } else if (SmolRTSP_Method_eq(&method, &describe)) {
        ret = VCALL(controller, describe, ctx, req);
    } else if (SmolRTSP_Method_eq(&method, &setup)) {
        ret = VCALL(controller, setup, ctx, req);
    } else if (SmolRTSP_Method_eq(&method, &play)) {
        ret = VCALL(controller, play, ctx, req);
    } else if (SmolRTSP_Method_eq(&method, &teardown)) {
        ret = VCALL(controller, teardown, ctx, req);
    } else {
        ret = VCALL(controller, unknown, ctx, req);
    }

    VCALL(controller, after, ret, ctx, req);

    VTABLE(SmolRTSP_Context, SmolRTSP_Droppable).drop(ctx);
}
