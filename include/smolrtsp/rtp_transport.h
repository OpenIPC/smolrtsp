/**
 * @file
 * An RTP data transport.
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/transport.h>

#include <stdbool.h>
#include <stdint.h>

#include <slice99.h>
#include <smolrtsp.h>

typedef struct SmolRTSP_RtpTransport SmolRTSP_RtpTransport;

SmolRTSP_RtpTransport *SmolRTSP_RtpTransport_new(SmolRTSP_Transport t, uint8_t rtsp_stream_id);

void SmolRTSP_RtpTransport_send_packet(
    SmolRTSP_RtpTransport *self, uint64_t timestamp_us, bool marker, uint8_t payload_ty,
    uint32_t clock_rate, U8Slice99 data_header, U8Slice99 data);

declImplExtern99(SmolRTSP_Droppable, SmolRTSP_RtpTransport);
