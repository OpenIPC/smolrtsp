/**
 * @file
 * @brief A request context.
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/message_body.h>
#include <smolrtsp/types/status_code.h>
#include <smolrtsp/writer.h>

#include <stdarg.h>
#include <stdint.h>

#include <interface99.h>
#include <slice99.h>

/**
 * An opaque type used to conveniently respond to RTSP clients.
 */
typedef struct SmolRTSP_Context SmolRTSP_Context;

/**
 * Creates a new SmolRTSP context.
 *
 * @param[in] w The writer to be provided with the response.
 * @param[in] cseq The sequence number for an RTSP request/response pair.
 *
 * @pre `w.self && w.vptr`
 */
SmolRTSP_Context *
SmolRTSP_Context_new(SmolRTSP_Writer w, uint32_t cseq) SMOLRTSP_PRIV_MUST_USE;

/**
 * Retrieves the writer specified in #SmolRTSP_Context_new.
 *
 * @pre `ctx != NULL`
 */
SmolRTSP_Writer
SmolRTSP_Context_get_writer(const SmolRTSP_Context *ctx) SMOLRTSP_PRIV_MUST_USE;

/**
 * Retrieves `cseq` specified in #SmolRTSP_Context_new.
 *
 * @pre `ctx != NULL`
 */
uint32_t
SmolRTSP_Context_get_cseq(const SmolRTSP_Context *ctx) SMOLRTSP_PRIV_MUST_USE;

/**
 * Retrieves the RTSP respond return value.
 *
 * If you have not responded yet, the result is 0.
 *
 * @pre `ctx != NULL`
 */
ssize_t
SmolRTSP_Context_get_ret(const SmolRTSP_Context *ctx) SMOLRTSP_PRIV_MUST_USE;

/**
 * Appends an RTSP header to the request context.
 *
 * @param[out] ctx The request context to modify.
 * @param[in] key The header key.
 * @param[in] fmt The `printf`-like format string (header value).
 * @param[in] list The variadic function arguments.
 *
 * @pre `ctx != NULL`
 * @pre @p ctx must contain strictly less than #SMOLRTSP_HEADER_MAP_CAPACITY
 * headers.
 * @pre `fmt != NULL`
 */
void smolrtsp_vheader(
    SmolRTSP_Context *ctx, CharSlice99 key, const char *restrict fmt,
    va_list list) SMOLRTSP_PRIV_GCC_ATTR(format(printf, 3, 0));

/**
 * The #smolrtsp_vheader twin.
 */
void smolrtsp_header(
    SmolRTSP_Context *ctx, CharSlice99 key, const char *restrict fmt, ...)
    SMOLRTSP_PRIV_GCC_ATTR(format(printf, 3, 4));

/**
 * Sets an RTSP body in the request context.
 *
 * @param[out] ctx The request context to modify.
 * @param[in] body The RTSP body.
 *
 * @pre `ctx != NULL`
 */
void smolrtsp_body(SmolRTSP_Context *ctx, SmolRTSP_MessageBody body);

/**
 * Writes an RTSP response to the underlying writer.
 *
 * The `CSeq` and `Content-Length` headers will be written automatically as
 * first headers.
 *
 * @param[out] ctx The request context to write the response to.
 * @param[in] code The RTSP status code.
 * @param[in] reason The RTSP reason phrase.
 *
 * @pre `ctx != NULL`
 * @pre @p reason is a null-terminated string.
 *
 * @return The number of bytes written or a negative value on error.
 */
ssize_t smolrtsp_respond(
    SmolRTSP_Context *ctx, SmolRTSP_StatusCode code, const char *reason);

/**
 * A shortcut for `smolrtsp_respond(ctx, SMOLRTSP_STATUS_OK, "OK")`.
 */
ssize_t smolrtsp_respond_ok(SmolRTSP_Context *ctx);

/**
 * A shortcut for `smolrtsp_respond(ctx, SMOLRTSP_STATUS_INTERNAL_SERVER_ERROR,
 * "Internal error")`.
 */
ssize_t smolrtsp_respond_internal_error(SmolRTSP_Context *ctx);

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_Context.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_Context);
