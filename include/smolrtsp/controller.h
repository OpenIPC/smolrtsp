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
 * This is the central high-level interface, which should be invoked only in
 * SmolRTSP integration code, such as
 * [OpenIPC/smolrtsp-libevent](https://github.com/OpenIPC/smolrtsp-libevent). A
 * simple usage example can be found
 * [here](https://github.com/OpenIPC/smolrtsp/blob/master/examples/server.c).
 *
 * All RTSP command handlers accept the following parameters:
 *  - `ctx` -- a request context to which you respond via #smolrtsp_respond. You
 * can set up response headers/body via #smolrtsp_header/#smolrtsp_body. Note
 * that you can also do it from within the `before` method, which is helpful for
 * setting up initial headers.
 *  - `req` -- a fully parsed request object.
 *
 * Command handlers return `ssize_t` -- the number of response bytes written
 * or a negative value on error (typically, it is a return value of
 * #smolrtsp_respond). Inside the `after` method, you will receive this
 * success/failure code and will be able to handle an error if it is <0.
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
