#ifndef SMOLRTSP_KNOWN_HEADERS_ACCEPT_RANGES_H
#define SMOLRTSP_KNOWN_HEADERS_ACCEPT_RANGES_H

#include <header.h>
#include <misc/range_unit.h>

typedef struct {
    SmolRTSP_HeaderVTable super;
    size_t range_units_count;
    const SmolRTSP_RangeUnit *range_units;
} SmolRTSP_HeaderAcceptRanges;

SmolRTSP_HeaderAcceptRanges
SmolRTSP_HeaderAcceptRanges_new(size_t range_units_count, const SmolRTSP_RangeUnit *range_units);

#endif // SMOLRTSP_KNOWN_HEADERS_ACCEPT_RANGES_H
