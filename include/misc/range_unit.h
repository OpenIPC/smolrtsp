#ifndef SMOLRTSP_MISC_RANGE_UNIT_H
#define SMOLRTSP_MISC_RANGE_UNIT_H

typedef enum {
    SmolRTSP_RangeUnitNpt,
    SmolRTSP_RangeUnitSmpte,
    SmolRTSP_RangeUnitSmpte30Drop,
    SmolRTSP_RangeUnitSmpte25,
    SmolRTSP_RangeUnitClock,
    SmolRTSP_RangeUnitExtensionFormat,
} SmolRTSP_RangeUnit;

const char *SmolRTSP_RangeUnit_stringify(SmolRTSP_RangeUnit range_unit);

#endif // SMOLRTSP_MISC_RANGE_UNIT_H
