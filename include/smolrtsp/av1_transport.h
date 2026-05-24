/**
 * @file
 * @brief An RTP/AV1 data transport.
 *
 * @see AOMedia AV1 RTP Specification:
 * <https://aomediacodec.github.io/av1-rtp-spec/>
 * @see AV1 Bitstream & Decoding Process Specification:
 * <https://aomediacodec.github.io/av1-spec/av1-spec.pdf>
 */

#pragma once

#include <smolrtsp/av1.h>
#include <smolrtsp/droppable.h>
#include <smolrtsp/rtp_transport.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The default value for #SmolRTSP_Av1TransportConfig.max_packet_size.
 *
 * Includes the 1-byte AV1 aggregation header: the OBU-element bytes
 * carried per RTP packet are at most
 * `max_packet_size - SMOLRTSP_AV1_AGGREGATION_HEADER_SIZE`.
 */
#define SMOLRTSP_MAX_AV1_PACKET_SIZE 1200

/**
 * The configuration structure for #SmolRTSP_Av1Transport.
 */
typedef struct {
    /**
     * Maximum size in bytes of the RTP payload (i.e. the 1-byte AV1
     * aggregation header plus the OBU bytes that fit after it) per RTP
     * packet. OBUs larger than this minus the header are split across
     * multiple packets with the Z/Y continuation flags.
     */
    size_t max_packet_size;
} SmolRTSP_Av1TransportConfig;

/**
 * Returns the default #SmolRTSP_Av1TransportConfig:
 * #SMOLRTSP_MAX_AV1_PACKET_SIZE.
 */
SmolRTSP_Av1TransportConfig
SmolRTSP_Av1TransportConfig_default(void) SMOLRTSP_PRIV_MUST_USE;

/**
 * An RTP/AV1 data transport.
 */
typedef struct SmolRTSP_Av1Transport SmolRTSP_Av1Transport;

/**
 * Creates a new RTP/AV1 transport with the default configuration.
 *
 * @param[in] t The underlying RTP transport.
 *
 * @pre `t != NULL`.
 */
SmolRTSP_Av1Transport *
SmolRTSP_Av1Transport_new(SmolRTSP_RtpTransport *t) SMOLRTSP_PRIV_MUST_USE;

/**
 * Creates a new RTP/AV1 transport with a custom configuration.
 *
 * @param[in] t The underlying RTP transport.
 * @param[in] config The transmission configuration structure.
 *
 * @pre `t != NULL`.
 * @pre `config.max_packet_size > SMOLRTSP_AV1_AGGREGATION_HEADER_SIZE + 1`.
 */
SmolRTSP_Av1Transport *SmolRTSP_Av1Transport_new_with_config(
    SmolRTSP_RtpTransport *t,
    SmolRTSP_Av1TransportConfig config) SMOLRTSP_PRIV_MUST_USE;

/**
 * Sends a complete AV1 temporal unit (TU) as a sequence of RTP packets.
 *
 * @p obus is the concatenated raw OBU stream for one TU exactly as
 * emitted by an AV1 encoder (each OBU prefixed by its OBU header byte,
 * optional extension header byte, and -- if `obu_has_size_field == 1`
 * -- a LEB128 `obu_size` field, followed by `obu_size` bytes of
 * payload). The library:
 *
 *   1. Parses the stream into OBUs.
 *   2. Drops OBUs of type #SMOLRTSP_AV1_OBU_TEMPORAL_DELIMITER and
 *      #SMOLRTSP_AV1_OBU_TILE_LIST per AOMedia AV1 RTP §5.
 *   3. Clears the `obu_has_size_field` bit and drops the `obu_size`
 *      LEB128 from each remaining OBU, per AOMedia AV1 RTP §4.5.
 *   4. Emits one RTP packet per OBU (`W=1`); if an OBU exceeds the MTU
 *      budget it is split across consecutive packets with the Z and Y
 *      continuation flags set per §4.4.
 *
 * The RTP marker bit is set on the very last packet of the TU. The N
 * (new coded video sequence) bit is set on the first packet only when
 * @p is_new_cvs is `true`; that packet must not be a continuation
 * (Z=0) per the §4.4 constraint.
 *
 * Each packet shares @p ts as its RTP timestamp.
 *
 * @param[in] self The transport.
 * @param[in] ts The RTP timestamp shared by every packet of the TU.
 * @param[in] obus The concatenated raw OBU bytes of this temporal unit.
 * @param[in] is_new_cvs `true` iff this TU begins a new coded video
 *            sequence (typically when it contains a sequence header
 *            OBU and the receiver should treat it as a sync point).
 *
 * @pre `self != NULL`.
 *
 * @return 0 on success; -1 on I/O error (with `errno` set) or on an
 *         OBU stream that fails to parse.
 */
int SmolRTSP_Av1Transport_send_temporal_unit(
    SmolRTSP_Av1Transport *self, SmolRTSP_RtpTimestamp ts, U8Slice99 obus,
    bool is_new_cvs) SMOLRTSP_PRIV_MUST_USE;

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_Av1Transport.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_Av1Transport);

bool SmolRTSP_Av1Transport_is_full(SmolRTSP_Av1Transport *self);
