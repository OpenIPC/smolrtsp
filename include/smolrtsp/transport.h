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
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
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
    vfunc99(int, transmit, VSelf99, SmolRTSP_IoVecSlice bufs)                  \
    vfunc99(bool, is_full, VSelf99)

/**
 * The superinterfaces of #SmolRTSP_Transport_IFACE.
 */
#define SmolRTSP_Transport_EXTENDS (SmolRTSP_Droppable)

/**
 * Defines the `SmolRTSP_Transport` interface.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
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
SmolRTSP_Transport smolrtsp_transport_tcp(
    SmolRTSP_Writer w, uint8_t channel_id,
    size_t max_buffer) SMOLRTSP_PRIV_MUST_USE;

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

/**
 * Creates a new UDP transport with address.
 *
 * @param[in] fd The socket file descriptor to be provided with data.
 * @param[in] addr Pointer to address data (e.g., sockaddr_un or sockaddr_in).
 * @param[in] len Length of the address in bytes.
 *
 * @pre `fd >= 0`
 * @pre `addr && len > 0`
 */
SmolRTSP_Transport smolrtsp_transport_udp_address(
    int fd, void *addr, size_t len) SMOLRTSP_PRIV_MUST_USE;

/**
 * Creates a new datagram socket suitable for #smolrtsp_transport_udp.
 *
 * The algorithm is:
 *  1. Create a socket using `socket(af, SOCK_DGRAM, 0)`.
 *  2. Connect this socket to @p addr with @p port.
 *  3. Set the `IP_PMTUDISC_WANT` option to allow IP fragmentation.
 *
 * @param[in] af The socket namespace. Can be `AF_INET` or `AF_INET6`; if none
 * of them, returns -1 and sets `errno` to `EAFNOSUPPORT`.
 * @param[in] addr The destination IP address: `struct in_addr` for `AF_INET`
 * and `struct in6_addr` for `AF_INET6`.
 * @param[in] port The destination IP port in the host byte order.
 *
 * @return A valid file descriptor or -1 on error (and sets `errno`
 * appropriately).
 */
int smolrtsp_dgram_socket(int af, const void *restrict addr, uint16_t port)
    SMOLRTSP_PRIV_MUST_USE;

/**
 * Returns a pointer to the IP address of @p addr.
 *
 * Currently, only `AF_INET` and `AF_INET6` are supported. Otherwise, `NULL` is
 * returned.
 *
 * @pre `addr != NULL`
 */
void *smolrtsp_sockaddr_ip(const struct sockaddr *restrict addr)
    SMOLRTSP_PRIV_MUST_USE;
