/**
 * @file
 * @brief An RTSP requests controller.
 */

#pragma once

#include <smolrtsp/context.h>
#include <smolrtsp/droppable.h>
#include <smolrtsp/types/request.h>
#include <smolrtsp/writer.h>

#include <stddef.h>

#include <interface99.h>

/**
 * A controller that handles incoming RTSP requests.
 *
 * All RTSP command handlers accept the following parameters:
 *  - `ctx` -- a request context used to respond to your RTSP client.
 *  - `req` -- a fully parsed request object.
 *
 * Command handlers return `ssize_t` -- the number of response bytes written
 * or a negative value on error (typically, it is a return value of
 * #smolrtsp_respond/#smolrtsp_respond_ok or similar).
 */
#define SmolRTSP_Controller_IFACE                                              \
                                                                               \
    /*                                                                         \
     * Handles `OPTIONS` as defined in                                         \
     * <https://datatracker.ietf.org/doc/html/rfc2326#section-10.1>.           \
     */                                                                        \
    vfunc99(                                                                   \
        ssize_t, options, VSelf99, SmolRTSP_Context *ctx,                      \
        const SmolRTSP_Request *req)                                           \
                                                                               \
    /*                                                                         \
     * Handles `OPTIONS` as defined in                                         \
     * <https://datatracker.ietf.org/doc/html/rfc2326#section-10.2>.           \
     */                                                                        \
    vfunc99(                                                                   \
        ssize_t, describe, VSelf99, SmolRTSP_Context *ctx,                     \
        const SmolRTSP_Request *req)                                           \
                                                                               \
    /*                                                                         \
     * Handles `OPTIONS` as defined in                                         \
     * <https://datatracker.ietf.org/doc/html/rfc2326#section-10.4>.           \
     */                                                                        \
    vfunc99(                                                                   \
        ssize_t, setup, VSelf99, SmolRTSP_Context *ctx,                        \
        const SmolRTSP_Request *req)                                           \
                                                                               \
    /*                                                                         \
     * Handles `OPTIONS` as defined in                                         \
     * <https://datatracker.ietf.org/doc/html/rfc2326#section-10.5>.           \
     */                                                                        \
    vfunc99(                                                                   \
        ssize_t, play, VSelf99, SmolRTSP_Context *ctx,                         \
        const SmolRTSP_Request *req)                                           \
                                                                               \
    /*                                                                         \
     * Handles `OPTIONS` as defined in                                         \
     * <https://datatracker.ietf.org/doc/html/rfc2326#section-10.7>.           \
     */                                                                        \
    vfunc99(                                                                   \
        ssize_t, teardown, VSelf99, SmolRTSP_Context *ctx,                     \
        const SmolRTSP_Request *req)                                           \
                                                                               \
    /*                                                                         \
     * Handles a command that is neither of the above.                         \
     */                                                                        \
    vfunc99(                                                                   \
        ssize_t, unknown, VSelf99, SmolRTSP_Context *ctx,                      \
        const SmolRTSP_Request *req)                                           \
                                                                               \
    /*                                                                         \
     * A method that is invoked _before_ the actual request handling.          \
     */                                                                        \
    vfunc99(                                                                   \
        void, before, VSelf99, SmolRTSP_Context *ctx,                          \
        const SmolRTSP_Request *req)                                           \
                                                                               \
    /*                                                                         \
     * A method that is invoked _after_ request handling.                      \
     */                                                                        \
    vfunc99(                                                                   \
        void, after, VSelf99, ssize_t ret, SmolRTSP_Context *ctx,              \
        const SmolRTSP_Request *req)

/**
 * The superinterfaces of #SmolRTSP_Controller_IFACE.
 */
#define SmolRTSP_Controller_EXTENDS (SmolRTSP_Droppable)

/**
 * Defines the `SmolRTSP_Controller` interface.
 *
 * See [Interface99](https://github.com/Hirrolot/interface99) for the macro
 * usage.
 */
interface99(SmolRTSP_Controller);

/**
 * Dispatches an incoming request to @p controller.
 *
 * The algorithm is as follows:
 *
 *  1. Setup a request context #SmolRTSP_Context. Later you can use it to
 * configure an RTSP response via #smolrtsp_header, #smolrtsp_body, and similar.
 *  2. Invoke the `before` method of @p controller. Here you should do some
 * preliminary stuff like logging a request or setting up initial response
 * headers via #smolrtsp_header.
 *  3. Invoke a corresponding command handler of @p controller. Here you should
 * handle the request and respond to your client via
 * #smolrtsp_respond/#smolrtsp_respond_ok or similar.
 *  4. Invoke the `after` method of @p controller. Here you should handle the
 * return value of a command handler invoked by #smolrtsp_dispatch. If it is <0,
 * it means that something bad happened so that the handler has not been able to
 * respond properly.
 *  5. Drop the request context.
 *
 * @param[out] conn The writer to send RTSP responses.
 * @param[in] controller The controller to handle the incoming request @p req.
 * @param[in] req The fully parsed RTSP request object.
 *
 * @pre `conn.self && conn.vptr`
 * @pre `controller.self && controller.vptr`
 * @pre `req != NULL`
 */
void smolrtsp_dispatch(
    SmolRTSP_Writer conn, SmolRTSP_Controller controller,
    const SmolRTSP_Request *restrict req);
