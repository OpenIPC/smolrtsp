/**
 * @file
 * @brief IP address-related routines.
 */

#pragma once

#include <netinet/in.h>
#include <stdint.h>

#include <datatype99.h>

/**
 * An IP address (V4/V6).
 *
 * See [Datatype99](https://github.com/Hirrolot/datatype99) for the macro usage.
 */

// clang-format off
datatype(
    SmolRTSP_IpAddr,
    (SmolRTSP_IpAddr_V4, struct in_addr),
    (SmolRTSP_IpAddr_V6, struct in6_addr)
);
// clang-format on

/**
 * Converts @p self to `struct sockaddr_storage`.
 */
struct sockaddr_storage
SmolRTSP_IpAddr_to_sockaddr(SmolRTSP_IpAddr self, uint16_t port);
