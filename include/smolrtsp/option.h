/**
 * @file
 * @brief Optional values.
 */

#pragma once

#include <datatype99.h>
#include <metalang99.h>

/**
 * Defines a typed optional value.
 *
 *  - `SMOLRTSP_DEF_OPTION(T)` expands to `datatype99(T##Option, (T##_Some, T),
 * (T##_None))`.
 *  - `SMOLRTSP_DEF_OPTION(name, T)` expands to `datatype99(name##Option,
 * (name##_Some, T), (name##_None))`.
 *
 * See [Datatype99](https://github.com/hirrolot/datatype99) for the macro usage.
 */
#define SMOLRTSP_DEF_OPTION(...)                                               \
    ML99_OVERLOAD(SMOLRTSP_PRIV_DEF_OPTION_, __VA_ARGS__)

#ifndef DOXYGEN_IGNORE

#define SMOLRTSP_PRIV_DEF_OPTION_1(T)                                          \
    datatype99(T##Option, (T##_Some, T), (T##_None))
#define SMOLRTSP_PRIV_DEF_OPTION_2(name, T)                                    \
    datatype99(name##Option, (name##_Some, T), (name##_None))

#endif // DOXYGEN_IGNORE
