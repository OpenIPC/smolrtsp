/**
 * @file
 * @brief Droppable types support.
 */

#pragma once

#include <interface99.h>

/**
 * Types that can be dropped (deallocated).
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
#define SmolRTSP_Droppable_IFACE                                               \
                                                                               \
    /*                                                                         \
     * Deallocates itself so that it can no longer be used.                    \
     */                                                                        \
    vfunc99(void, drop, VSelf99)

/**
 * Defines the `SmolRTSP_Droppable` interface.
 *
 * See [Interface99](https://github.com/hirrolot/interface99) for the macro
 * usage.
 */
interface99(SmolRTSP_Droppable);
