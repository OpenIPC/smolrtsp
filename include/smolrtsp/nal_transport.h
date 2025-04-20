/**
 * @file
 * @brief An RTP/NAL data transport.
 *
 * @see RTP Payload Format for H.264 Video:
 * <https://datatracker.ietf.org/doc/html/rfc6184>
 * @see RTP Payload Format for High Efficiency Video Coding (HEVC):
 * <https://datatracker.ietf.org/doc/html/rfc7798>
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/nal.h>
#include <smolrtsp/rtp_transport.h>

#include <stddef.h>
#include <stdint.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The default value for #SmolRTSP_NalTransportConfig.max_h264_nalu_size.
 */
#define SMOLRTSP_MAX_H264_NALU_SIZE 1200

/**
 * The default value for #SmolRTSP_NalTransportConfig.max_h265_nalu_size.
 */
#define SMOLRTSP_MAX_H265_NALU_SIZE 1200

/**
 * The configuration structure for #SmolRTSP_NalTransport.
 */
typedef struct {
    /**
     * The maximum size of an H.264 NAL unit (including the header).
     */
    size_t max_h264_nalu_size;

    /**
     * The maximum size of an H.265 NAL unit (including the header).
     */
    size_t max_h265_nalu_size;
} SmolRTSP_NalTransportConfig;

/**
 * Returns the default #SmolRTSP_NalTransportConfig.
 *
 * The default values are:
 *
 *  - `max_h264_nalu_size` is #SMOLRTSP_MAX_H264_NALU_SIZE.
 *  - `max_h265_nalu_size` is #SMOLRTSP_MAX_H265_NALU_SIZE.
 */
SmolRTSP_NalTransportConfig
SmolRTSP_NalTransportConfig_default(void) SMOLRTSP_PRIV_MUST_USE;

/**
 * An RTP/NAL data transport.
 */
typedef struct SmolRTSP_NalTransport SmolRTSP_NalTransport;

/**
 * Creates a new RTP/NAL transport with the default configuration.
 *
 * @param[in] t The underlying RTP transport.
 *
 * @pre `t != NULL`
 */
SmolRTSP_NalTransport *
SmolRTSP_NalTransport_new(SmolRTSP_RtpTransport *t) SMOLRTSP_PRIV_MUST_USE;

/**
 * Creates a new RTP/NAL transport with a custom configuration.
 *
 * @param[in] t The underlying RTP transport.
 * @param[in] config The transmission configuration structure.
 *
 * @pre `t != NULL`
 */
SmolRTSP_NalTransport *SmolRTSP_NalTransport_new_with_config(
    SmolRTSP_RtpTransport *t,
    SmolRTSP_NalTransportConfig config) SMOLRTSP_PRIV_MUST_USE;

/**
 * Sends an RTP/NAL packet.
 *
 * If @p nalu is larger than the limit values from #SmolRTSP_NalTransportConfig
 * (configured via #SmolRTSP_NalTransport_new),
 * @p nalu will be
 * [fragmented](https://datatracker.ietf.org/doc/html/rfc6184#section-5.8).
 *
 * @param[out] self The RTP/NAL transport for sending this packet.
 * @param[in] ts The RTP timestamp for this packet.
 * @param[in] nalu The NAL unit of this RTP packet.
 *
 * @pre `self != NULL`
 *
 * @return -1 if an I/O error occurred and sets `errno` appropriately, 0 on
 * success.
 */
int SmolRTSP_NalTransport_send_packet(
    SmolRTSP_NalTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_NalUnit nalu) SMOLRTSP_PRIV_MUST_USE;

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_NalTransport.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_NalTransport);

bool SmolRTSP_NalTransport_is_full(SmolRTSP_NalTransport *self);
