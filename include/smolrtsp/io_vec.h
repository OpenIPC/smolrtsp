/**
 * @file
 * Vectored I/O support.
 */

#pragma once

#include <stddef.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <slice99.h>

/**
 * A slice of elements of type `struct iovec`.
 */
SLICE99_DEF_TYPED(SmolRTSP_IoVecSlice, struct iovec);

/**
 * Computes the total length of @p self.
 */
size_t SmolRTSP_IoVecSlice_len(SmolRTSP_IoVecSlice self);
