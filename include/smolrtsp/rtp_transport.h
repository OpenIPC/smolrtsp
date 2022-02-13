/**
 * @file
 * An RTP data transport.
 *
 * TODO: more docs.
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/transport.h>

#include <stdbool.h>
#include <stdint.h>

#include <slice99.h>
#include <smolrtsp.h>

/**
 * An RTP data transport.
 */
typedef struct SmolRTSP_RtpTransport SmolRTSP_RtpTransport;

/**
 * Creates a new RTP transport from the underlying level-4 protocol @p t.
 */
SmolRTSP_RtpTransport *SmolRTSP_RtpTransport_new(SmolRTSP_Transport t, uint8_t rtsp_stream_id);

/**
 * Sends an RTP packet.
 */
void SmolRTSP_RtpTransport_send_packet(
    SmolRTSP_RtpTransport *self, uint64_t timestamp_us, bool marker, uint8_t payload_ty,
    uint32_t clock_rate, U8Slice99 data_header, U8Slice99 data);

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_RtpTransport.
 *
 * See [Interface99](https://github.com/Hirrolot/interface99) for the macro usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_RtpTransport);
