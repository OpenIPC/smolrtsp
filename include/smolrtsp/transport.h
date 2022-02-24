/**
 * @file
 * @brief RTSP data transport (level 4) implementations.
 */

#pragma once

#include <smolrtsp/droppable.h>
#include <smolrtsp/io_vec.h>
#include <smolrtsp/writer.h>

#include <interface99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * A transport-level RTSP data transmitter.
 *
 * See [Interface99](https://github.com/Hirrolot/interface99) for the macro
 * usage.
 */
#define SmolRTSP_Transport_IFACE                                               \
                                                                               \
    /*                                                                         \
     * Transmits a slice of I/O vectors @p bufs.                               \
     *                                                                         \
     * @return -1 if an I/O error occurred and sets `errno` appropriately, 0   \
     * on success.                                                             \
     */                                                                        \
    vfunc99(int, transmit, VSelf99, SmolRTSP_IoVecSlice bufs)

#define SmolRTSP_Transport_EXTENDS (SmolRTSP_Droppable)

/**
 * Defines the `SmolRTSP_Transport` interface.
 *
 * See [Interface99](https://github.com/Hirrolot/interface99) for the macro
 * usage.
 */
interface99(SmolRTSP_Transport);

/**
 * Creates a new TCP transport.
 *
 * @param[in] w The writer to be provided with data.
 * @param[in] channel_id A one-byte channel identifier, as defined in
 * <https://datatracker.ietf.org/doc/html/rfc2326#section-10.12>.
 *
 * @pre `w.self && w.vptr`
 */
SmolRTSP_Transport smolrtsp_transport_tcp(SmolRTSP_Writer w, uint8_t channel_id)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Creates a new UDP transport.
 *
 * Strictly speaking, it can handle any datagram-oriented protocol, not
 * necessarily UDP. E.g., you may use a `SOCK_SEQPACKET` socket for local
 * communication.
 *
 * @param[in] fd The socket file descriptor to be provided with data.
 *
 * @pre `fd >= 0`
 */
SmolRTSP_Transport smolrtsp_transport_udp(int fd) SMOLRTSP_PRIV_MUST_USE;
