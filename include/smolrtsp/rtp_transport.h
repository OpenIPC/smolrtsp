/**
 * @file
 * @brief An RTP data transport.
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/transport.h>

#include <stdbool.h>
#include <stdint.h>

#include <datatype99.h>
#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * An RTP data transport.
 */
typedef struct SmolRTSP_RtpTransport SmolRTSP_RtpTransport;

/**
 * An RTP timestamp.
 *
 * ## Variants
 *
 *  - `Raw` -- The value to be assigned to #SmolRTSP_RtpHeader.timestamp without
 * further conversion.
 *  - `SysClockUs` -- The timestamp value in microseconds derived from a system
 * clock (e.g., `clock_gettime`). It should be used when a raw timestamp cannot
 * be computed, as typically occurs with real-time video.
 *
 * See [Datatype99](https://github.com/hirrolot/datatype99) for the macro usage.
 */

// clang-format off
datatype99(
    SmolRTSP_RtpTimestamp,
    (SmolRTSP_RtpTimestamp_Raw, uint32_t),
    (SmolRTSP_RtpTimestamp_SysClockUs, uint64_t)
);
// clang-format on

/**
 * Creates a new RTP transport from the underlying level-4 protocol @p t.
 *
 * @param[in] t The level-4 protocol (such as TCP or UDP).
 * @param[in] payload_ty The RTP payload type. The list of payload types is
 * available here: <https://en.wikipedia.org/wiki/RTP_payload_formats>.
 * @param[in] clock_rate The RTP clock rate of @p payload_ty (HZ).
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
 * @param[in] ts The RTP timestamp for this packet.
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
    SmolRTSP_RtpTransport *self, SmolRTSP_RtpTimestamp ts, bool marker,
    U8Slice99 payload_header, U8Slice99 payload) SMOLRTSP_PRIV_MUST_USE;

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_RtpTransport.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_RtpTransport);

bool SmolRTSP_RtpTransport_is_full(SmolRTSP_RtpTransport *self);
