#include <known_headers/accept_ranges.h>

#include <string.h>

static const char *SmolRTSP_HeaderAcceptRanges_key(void) {
    return "Accept-Ranges";
}

static void SmolRTSP_HeaderAcceptRanges_write_value(
    const SmolRTSP_HeaderAcceptRanges *self, SmolRTSP_Writer writer, const void *user_cx) {
    for (size_t i = 0; i < self->range_units_count; i++) {
        const char *range_unit = SmolRTSP_RangeUnit_stringify(self->range_units[i]);
        writer(strlen(range_unit), (const void *)range_unit, user_cx);
    }
}

SMOLRTSP_VTABLE(SmolRTSP_Header, SmolRTSP_HeaderAcceptRanges) = {
    .key = SmolRTSP_HeaderAcceptRanges_key,
    .write_value = SmolRTSP_HeaderAcceptRanges_write_value,
};
