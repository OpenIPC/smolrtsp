#include <known_headers/accept_ranges.h>

#include <string.h>

static const char *SmolRTSP_HeaderAcceptRanges_key(void) {
    return "Accept-Ranges";
}

static void SmolRTSP_HeaderAcceptRanges_write_value(
    const SmolRTSP_HeaderAcceptRanges *self, SmolRTSP_Writer writer, const void *user_cx) {
    for (size_t i = 0; i < self->range_units_count; i++) {
        const char *range_unit = smolrtsp_range_unit_stringify(self->range_units[i]);
        writer(strlen(range_unit), (const void *)range_unit, user_cx);
    }
}

static const SmolRTSP_HeaderVTable SmolRTSP_HeaderAcceptRanges_vtable = {
    .key = SmolRTSP_HeaderAcceptRanges_key,
    .write_value = SmolRTSP_HeaderAcceptRanges_write_value,
};

SmolRTSP_HeaderAcceptRanges
SmolRTSP_HeaderAcceptRanges_new(size_t range_units_count, const SmolRTSP_RangeUnit *range_units) {
    return (SmolRTSP_HeaderAcceptRanges){
        .range_units_count = range_units_count,
        .range_units = range_units,
    };
}

SmolRTSP_Header SmolRTSP_HeaderAcceptRanges_base(const SmolRTSP_HeaderAcceptRanges *self) {
    return (SmolRTSP_Header){
        .vptr = &SmolRTSP_HeaderAcceptRanges_vtable,
        .self = (const void *)self,
    };
}
