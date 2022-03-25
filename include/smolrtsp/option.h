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