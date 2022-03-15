/**
 * @file
 * @brief An RTP data transport.
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/transport.h>

#include <stdbool.h>
#include <stdint.h>

#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * An RTP data transport.
 */
typedef struct SmolRTSP_RtpTransport SmolRTSP_RtpTransport;

/**
 * Creates a new RTP transport from the underlying level-4 protocol @p t.
 *
 * @param[in] t The level-4 protocol (such as TCP or UDP).
 * @param[in] payload_ty The RTP payload type. The list of payload types is
 * available here: <https://en.wikipedia.org/wiki/RTP_payload_formats>.
 * @param[in] clock_rate The RTP clock rate of @p payload_ty.
 *
 * @pre `t.self && t.vptr`
 * @pre The `rand` PRNG must be set up via `srand`.
 */
SmolRTSP_RtpTransport *SmolRTSP_RtpTransport_new(
    SmolRTSP_Transport t, uint8_t payload_ty,
    uint32_t clock_rate) SMOLRTSP_PRIV_MUST_USE;

/**
 * Sends an RTP packet.
 *
 * @param[out] self The RTP transport for sending this packet.
 * @param[in] timestamp_us The number of microseconds passed since the last
 * packet.
 * @param[in] marker The RTP marker flag.
 * @param[in] payload_header The payload header. Can be `U8Slice99_empty()`.
 * @param[in] payload The payload data.
 *
 * @pre `self != NULL`
 *
 * @return -1 if an I/O error occurred and sets `errno` appropriately, 0 on
 * success.
 */
int SmolRTSP_RtpTransport_send_packet(
    SmolRTSP_RtpTransport *self, uint64_t timestamp_us, bool marker,
    U8Slice99 payload_header, U8Slice99 payload) SMOLRTSP_PRIV_MUST_USE;

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_RtpTransport.
 *
 * See [Interface99](https://github.com/Hirrolot/interface99) for the macro
 * usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_RtpTransport);
