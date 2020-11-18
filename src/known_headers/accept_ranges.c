#include <known_headers/accept_ranges.h>

#include <string.h>

static const char *smolrtsp_header_accept_ranges_vtable_key(void) {
    return "Accept-Ranges";
}

static void smolrtsp_header_accept_ranges_vtable_write_value(
    const void *self, SmolRTSP_Writer writer, const void *user_cx) {
    {
        const SmolRTSP_HeaderAcceptRanges *self = (const SmolRTSP_HeaderAcceptRanges *)self;

        for (size_t i = 0; i < self->range_units_count; i++) {
            const char *range_unit = smolrtsp_range_unit_stringify(self->range_units[i]);
            writer(strlen(range_unit), (const void *)range_unit, user_cx);
        }
    }
}

static const SmolRTSP_HeaderVTable smolrtsp_header_accepts_ranges_vtable = {
    .key = smolrtsp_header_accept_ranges_vtable_key,
    .write_value = smolrtsp_header_accept_ranges_vtable_write_value,
};

SmolRTSP_HeaderAcceptRanges smolrtsp_header_accepts_ranges_new(
    size_t range_units_count, const SmolRTSP_RangeUnit *range_units) {
    return (SmolRTSP_HeaderAcceptRanges){
        .range_units_count = range_units_count,
        .range_units = range_units,
    };
}

SmolRTSP_Header smolrtsp_header_accepts_ranges_to_base(const SmolRTSP_HeaderAcceptRanges *header) {
    return (SmolRTSP_Header){
        .vptr = &smolrtsp_header_accepts_ranges_vtable,
        .self = (const void *)header,
    };
}
