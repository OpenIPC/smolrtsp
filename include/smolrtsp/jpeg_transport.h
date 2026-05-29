/**
 * @file
 * @brief An RTP/JPEG (Motion JPEG) data transport.
 *
 * @see RTP Payload Format for JPEG-compressed Video:
 * <https://datatracker.ietf.org/doc/html/rfc2435>
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/jpeg.h>
#include <smolrtsp/rtp_transport.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * The configuration structure for #SmolRTSP_JpegTransport.
 */
typedef struct {
    /**
     * Maximum size in bytes of the RTP payload (i.e. the RFC 2435 main
     * header plus the optional quantization-table block plus the scan-data
     * chunk) emitted per RTP packet. Frames larger than this are split
     * across multiple RTP packets; the first packet of a frame carries
     * the quantization-table block (when present) and so reserves more of
     * its budget for headers than continuation packets.
     */
    size_t max_packet_size;
} SmolRTSP_JpegTransportConfig;

/**
 * Returns the default #SmolRTSP_JpegTransportConfig
 * (#SMOLRTSP_MAX_JPEG_PACKET_SIZE).
 */
SmolRTSP_JpegTransportConfig
SmolRTSP_JpegTransportConfig_default(void) SMOLRTSP_PRIV_MUST_USE;

/**
 * An RTP/JPEG data transport.
 */
typedef struct SmolRTSP_JpegTransport SmolRTSP_JpegTransport;

/**
 * One Motion-JPEG frame, ready to packetize per RFC 2435.
 *
 * The transport leaves @ref SmolRTSP_JpegPayloadHeader.fragment_offset alone
 * in @ref hdr and stamps the actual offset per RTP packet -- callers may
 * initialize it to 0 and forget about it.
 *
 * If both @ref qt0 and @ref qt1 are empty, no quantization-table block is
 * emitted (RFC 2435 §4.1 referenced-tables case; Q < #SMOLRTSP_JPEG_Q_DYNAMIC).
 * Otherwise the first RTP packet of the frame carries a
 * #SmolRTSP_JpegQtHeader followed by the concatenation of the two slices
 * (RFC 2435 §3.1.8). Two 64-byte 8-bit tables (`qt0.len + qt1.len == 128`)
 * are the baseline case; a single grayscale table is acceptable (set
 * @ref qt1 to #U8Slice99_empty()).
 */
typedef struct {
    /**
     * Per-frame fields of the RFC 2435 main header: type, q, width_blocks,
     * height_blocks, type_specific. @ref
     * SmolRTSP_JpegPayloadHeader.fragment_offset is overwritten per packet.
     */
    SmolRTSP_JpegPayloadHeader hdr;
    /**
     * First quantization table, typically 64 bytes (8-bit baseline). Pass
     * #U8Slice99_empty() when no embedded QT is sent.
     */
    U8Slice99 qt0;
    /**
     * Second quantization table, typically 64 bytes. Pass #U8Slice99_empty()
     * for grayscale or single-table frames.
     */
    U8Slice99 qt1;
    /**
     * JPEG entropy-coded scan data (the bytes after the SOS marker, not
     * including the trailing EOI). May be empty -- a degenerate frame
     * still emits one RTP packet with the marker bit set.
     */
    U8Slice99 scan_data;
} SmolRTSP_JpegFrame;

/**
 * Creates a new RTP/JPEG transport with the default configuration.
 *
 * @param[in] t The underlying RTP transport.
 *
 * @pre `t != NULL`.
 */
SmolRTSP_JpegTransport *
SmolRTSP_JpegTransport_new(SmolRTSP_RtpTransport *t) SMOLRTSP_PRIV_MUST_USE;

/**
 * Creates a new RTP/JPEG transport with a custom configuration.
 *
 * @param[in] t The underlying RTP transport.
 * @param[in] config The transmission configuration structure.
 *
 * @pre `t != NULL`.
 * @pre `config.max_packet_size > SMOLRTSP_JPEG_MAIN_HEADER_SIZE`.
 */
SmolRTSP_JpegTransport *SmolRTSP_JpegTransport_new_with_config(
    SmolRTSP_RtpTransport *t,
    SmolRTSP_JpegTransportConfig config) SMOLRTSP_PRIV_MUST_USE;

/**
 * Sends one Motion-JPEG frame as a sequence of RTP packets.
 *
 * The library splits @ref SmolRTSP_JpegFrame.scan_data across as many RTP
 * packets as needed and builds the RFC 2435 §3.1 main header per packet,
 * stamping #SmolRTSP_JpegPayloadHeader.fragment_offset with the byte
 * offset inside the scan data carried by each packet. The first packet of
 * the frame also carries the quantization-table block (when @ref
 * SmolRTSP_JpegFrame.qt0 / @ref SmolRTSP_JpegFrame.qt1 are non-empty). The
 * RTP marker bit is set on the final packet of the frame.
 *
 * @param[in] self The transport.
 * @param[in] ts The RTP timestamp shared by every packet of the frame.
 * @param[in] frame The Motion-JPEG frame to send.
 *
 * @pre `self != NULL`.
 * @pre `frame.qt0.len + frame.qt1.len <= 0xFFFF` (fits the QT header
 *      Length field).
 * @pre `config.max_packet_size` is large enough to carry one main JPEG
 *      header plus the QT block (when present) plus at least one byte of
 *      scan data on the first packet -- a single-byte first packet is
 *      otherwise unrepresentable.
 *
 * @return 0 on success; -1 on I/O error (with `errno` set by the
 * underlying transport's send).
 */
int SmolRTSP_JpegTransport_send_frame(
    SmolRTSP_JpegTransport *self, SmolRTSP_RtpTimestamp ts,
    SmolRTSP_JpegFrame frame) SMOLRTSP_PRIV_MUST_USE;

/**
 * Implements #SmolRTSP_Droppable_IFACE for #SmolRTSP_JpegTransport.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
declImplExtern99(SmolRTSP_Droppable, SmolRTSP_JpegTransport);

bool SmolRTSP_JpegTransport_is_full(SmolRTSP_JpegTransport *self);
