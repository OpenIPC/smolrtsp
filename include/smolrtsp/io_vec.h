/**
 * @file
 * @brief Vectored I/O support.
 */

#pragma once

#include <stddef.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <slice99.h>

#include <smolrtsp/priv/compiler_attrs.h>

/**
 * A slice of elements of type `struct iovec`.
 */
SLICE99_DEF_TYPED(SmolRTSP_IoVecSlice, struct iovec);

/**
 * Computes the total length of @p self.
 */
size_t SmolRTSP_IoVecSlice_len(SmolRTSP_IoVecSlice self) SMOLRTSP_PRIV_MUST_USE;

/**
 * Converts an octet slice @p slice to `struct iovec`.
 */
struct iovec smolrtsp_slice_to_iovec(U8Slice99 slice) SMOLRTSP_PRIV_MUST_USE;
