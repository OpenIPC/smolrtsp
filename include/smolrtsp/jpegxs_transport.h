/**
 * @file
 * @brief An RTP/JPEG XS data transport.
 *
 * @see RTP Payload Format for ISO/IEC 21122 (JPEG XS):
 * <https://datatracker.ietf.org/doc/html/rfc9134>
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/jpegxs.h>
#include <smolrtsp/rtp_transport.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The default value for #SmolRTSP_JpegXsTransportConfig.max_packet_size.
 *
 * The limit includes the 4-byte JPEG XS payload header: the codestream
 * chunk per RTP packet is at most
 * `max_packet_size - SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE` bytes.
 */
#define SMOLRTSP_MAX_JPEGXS_PACKET_SIZE 1200

/**
 * The configuration structure for #SmolRTSP_JpegXsTransport.
 */
typedef struct {
    /**
     * Packetization mode (the K bit) used by every packet emitted by this
     * transport.
     */
    SmolRTSP_JpegXsPackMode pack_mode;

    /**
     * Transmission-order mode (the T bit). RFC 9134 §4.3 requires
     * `sequential == true` when @ref pack_mode is
     * #SMOLRTSP_JPEGXS_PACK_CODESTREAM.
     */
    bool sequential;

    /**
     * Maximum size in bytes of the RTP payload (i.e. 4-byte JPEG XS
     * payload header plus the codestream/slice chunk) per RTP packet.
     * Units larger than `max_packet_size -
     * SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE` bytes are split across
     * multiple RTP packets.
     */
    size_t max_packet_size;
} SmolRTSP_JpegXsTransportConfig;

/**
 * Returns the default #SmolRTSP_JpegXsTransportConfig:
 * codestream packetization mode, T=1, and #SMOLRTSP_MAX_JPEGXS_PACKET_SIZE.
 */
SmolRTSP_JpegXsTransportConfig
SmolRTSP_JpegXsTransportConfig_default(void) SMOLRTSP_PRIV_MUST_USE;

/**
 * An RTP/JPEG XS data transport.
 */
typedef struct SmolRTSP_JpegXsTransport SmolRTSP_JpegXsTransport;

/**
 * Creates a new RTP/JPEG XS transport with the default configuration.
 *
 * @param[in] t The underlying RTP transport.
 *
 * @pre `t != NULL`.
 */
SmolRTSP_JpegXsTransport *
SmolRTSP_JpegXsTransport_new(SmolRTSP_RtpTransport *t) SMOLRTSP_PRIV_MUST_USE;

/**
 * Creates a new RTP/JPEG XS transport with a custom configuration.
 *
 * @param[in] t The underlying RTP transport.
 * @param[in] config The transmission configuration structure.
 *
 * @pre `t != NULL`.
 * @pre `config.max_packet_size > SMOLRTSP_JPEGXS_PAYLOAD_HEADER_SIZE`.
 * @pre `config.sequential` is `true` when `config.pack_mode` is
 *      #SMOLRTSP_JPEGXS_PACK_CODESTREAM (RFC 9134 §4.3).
 */
SmolRTSP_JpegXsTransport *SmolRTSP_JpegXsTransport_new_with_config(
    SmolRTSP_RtpTransport *t,
    SmolRTSP_JpegXsTransportConfig config) SMOLRTSP_PRIV_MUST_USE;

/**
 * Sends a JPEG XS packetization unit in codestream mode (K=0).
 *
 * One call corresponds to one packetization unit: an entire progressive
 * frame, or a single field of an interlaced frame. The library splits
 * @p codestream across as many RTP packets as needed and builds the
 * RFC 9134 §4.3 payload header per packet. The L bit is set on the
 * final packet of the unit; the RTP marker bit is set only when
 * @p is_frame_end is `true` and on that same final packet. The internal
 * frame counter is post-incremented after a successful send with
 * @p is_frame_end true.
 *
 * @param[in] self The transport.
 * @param[in] ts The RTP timestamp shared by every packet of the unit.
 * @param[in] interlace The interlaced-information field.
 * @param[in] is_frame_end `true` iff this packetization unit ends the
 *            current frame -- always `true` for a progressive frame,
 *            `true` only on #SMOLRTSP_JPEGXS_INTERLACE_FIELD_2 for an
 *            interlaced frame.
 * @param[in] codestream The codestream bytes for this unit.
 *
 * @pre `self != NULL`.
 * @pre The configured pack mode is #SMOLRTSP_JPEGXS_PACK_CODESTREAM.
 *
 * @return 0 on success; -1 on I/O error (with `errno` set).
 */
int SmolRTSP_JpegXsTransport_send_codestream(
    SmolRTSP_JpegXsTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegXsInterlace interlace, bool is_frame_end,
    U8Slice99 codestream) SMOLRTSP_PRIV_MUST_USE;

/**
 * Sends the header-segment packetization unit in slice mode (K=1).
 *
 * The unit carries the VS box, CS box, and codestream-header segment;
 * every packet emitted by this call has SEP set to
 * #SMOLRTSP_JPEGXS_SEP_HEADER_SEGMENT. The RTP marker bit is never set.
 *
 * @param[in] self The transport.
 * @param[in] ts The RTP timestamp shared by every packet of the unit.
 * @param[in] interlace The interlaced-information field.
 * @param[in] header_segment The header-segment bytes.
 *
 * @pre `self != NULL`.
 * @pre The configured pack mode is #SMOLRTSP_JPEGXS_PACK_SLICE.
 *
 * @return 0 on success; -1 on I/O error (with `errno` set).
 */
int SmolRTSP_JpegXsTransport_send_header_segment(
    SmolRTSP_JpegXsTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegXsInterlace interlace,
    U8Slice99 header_segment) SMOLRTSP_PRIV_MUST_USE;

/**
 * Sends a single slice as its own packetization unit in slice mode (K=1).
 *
 * @param[in] self The transport.
 * @param[in] ts The RTP timestamp shared by every packet of the slice.
 * @param[in] interlace The interlaced-information field.
 * @param[in] slice_index The slice index in `[0, 0x7FE]`. (0x7FF is
 *            reserved for the header segment.) Placed in the SEP field.
 * @param[in] is_frame_end `true` iff this slice ends the current frame
 *            -- the RTP marker bit fires on the slice's final packet.
 *            For interlaced frames this should be `true` only on the
 *            last slice of #SMOLRTSP_JPEGXS_INTERLACE_FIELD_2.
 * @param[in] slice The slice bytes.
 *
 * @pre `self != NULL`.
 * @pre The configured pack mode is #SMOLRTSP_JPEGXS_PACK_SLICE.
 * @pre `slice_index < SMOLRTSP_JPEGXS_SEP_HEADER_SEGMENT`.
 *
 * @return 0 on success; -1 on I/O error (with `errno` set).
 */
int SmolRTSP_JpegXsTransport_send_slice(
    SmolRTSP_JpegXsTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegXsInterlace interlace, uint16_t slice_index, bool is_frame_end,
    U8Slice99 slice) SMOLRTSP_PRIV_MUST_USE;

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_JpegXsTransport.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_JpegXsTransport);

bool SmolRTSP_JpegXsTransport_is_full(SmolRTSP_JpegXsTransport *self);
