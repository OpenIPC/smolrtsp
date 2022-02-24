/**
 * @file
 * @brief An RTP/NAL data transport.
 *
 * @see RTP Payload Format for H.264 Video:
 * <https://datatracker.ietf.org/doc/html/rfc6184>
 * @see RTP Payload Format for High Efficiency Video Coding (HEVC):
 * <https://datatracker.ietf.org/doc/html/rfc7798>
 *
 * TODO: more docs.
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/nal/nal.h>
#include <smolrtsp/rtp_transport.h>

#include <stdint.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * An RTP/NAL data transport.
 */
typedef struct SmolRTSP_NalTransport SmolRTSP_NalTransport;

/**
 * Creates a new RTP/NAL transport.
 *
 * @pre `t != NULL`
 */
SmolRTSP_NalTransport *
SmolRTSP_NalTransport_new(SmolRTSP_RtpTransport *t) SMOLRTSP_PRIV_MUST_USE;

/**
 * Sends an RTP/NAL packet.
 *
 * @pre `self != NULL`
 *
 * @return -1 if an I/O error occurred and sets `errno` appropriately, 0 on
 * success.
 */
int SmolRTSP_NalTransport_send_packet(
    SmolRTSP_NalTransport *self, uint64_t timestamp_us, uint8_t payload_type,
    SmolRTSP_NalUnit nal_unit) SMOLRTSP_PRIV_MUST_USE;

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_NalTransport.
 *
 * See [Interface99](https://github.com/Hirrolot/interface99) for the macro
 * usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_NalTransport);
