#ifndef SMOLRTSP_KNOWN_HEADERS_ACCEPT_RANGES_H
#define SMOLRTSP_KNOWN_HEADERS_ACCEPT_RANGES_H

#include <header.h>
#include <misc/range_unit.h>

typedef struct {
    size_t range_units_count;
    const SmolRTSP_RangeUnit *range_units;
} SmolRTSP_HeaderAcceptRanges;

SMOLRTSP_VTABLE(SmolRTSP_Header, SmolRTSP_HeaderAcceptRanges);

#endif // SMOLRTSP_KNOWN_HEADERS_ACCEPT_RANGES_H
