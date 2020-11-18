#include <misc/range_unit.h>

const char *SmolRTSP_RangeUnit_stringify(SmolRTSP_RangeUnit range_unit) {
#define ASSOC(tag, string)                                                                         \
    case SmolRTSP_RangeUnit##tag:                                                                  \
        return string

    switch (range_unit) {
        ASSOC(Npt, "npt");
        ASSOC(Smpte, "smpte");
        ASSOC(Smpte30Drop, "smpte-30-drop");
        ASSOC(Smpte25, "smpte-25");
        ASSOC(Clock, "clock");
        ASSOC(ExtensionFormat, "extension-format");
    }

#undef ASSOC
}
