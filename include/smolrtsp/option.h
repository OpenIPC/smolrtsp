/**
 * @file
 * @brief Optional values.
 */

#pragma once

#include <datatype99.h>

/**
 * Defines an optional value of type @p T.
 *
 * See [Datatype99](https://github.com/Hirrolot/datatype99) for the macro usage.
 */
#define SMOLRTSP_DEF_OPTION(T) datatype99(T##Option, (T##_Some, T), (T##_None))

/**
 * The same as #SMOLRTSP_DEF_OPTION but allows to choose a custom typename.
 */
#define SMOLRTSP_DEF_OPTION_WITH_NAME(name, T)                                 \
    datatype99(name##Option, (name##_Some, T), (name##_None))
