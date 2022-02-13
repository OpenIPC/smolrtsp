#include <smolrtsp/types/error.h>

const char *SmolRTSP_ParseType_str(SmolRTSP_ParseType self) {
    switch (self) {
    case SmolRTSP_ParseType_Int:
        return "Integer";
    case SmolRTSP_ParseType_Ident:
        return "Identifier";
    case SmolRTSP_ParseType_HeaderName:
        return "Header name";
    default:
        return "Unknown";
    }
}

#define MAX_STR           10
#define TRUNCATE_STR(str) ((str).len <= MAX_STR ? (str) : CharSlice99_sub((str), 0, MAX_STR))

void SmolRTSP_ParseError_print(SmolRTSP_ParseError self, SmolRTSP_Writer w) {
    match(self) {
        of(SmolRTSP_ParseError_ContentLength, value) {
            SMOLRTSP_WRITE_SLICES(
                w, {
                       CharSlice99_from_str("Invalid Content-Length `"),
                       TRUNCATE_STR(*value),
                       CharSlice99_from_str("`."),
                   });
        }
        of(SmolRTSP_ParseError_StrMismatch, expected, actual) {
            SMOLRTSP_WRITE_SLICES(
                w, {
                       CharSlice99_from_str("String mismatch: expected `"),
                       TRUNCATE_STR(*expected),
                       CharSlice99_from_str("`, found `"),
                       TRUNCATE_STR(*actual),
                       CharSlice99_from_str("`."),
                   });
        }
        of(SmolRTSP_ParseError_TypeMismatch, kind, str) {
            SMOLRTSP_WRITE_SLICES(
                w, {
                       CharSlice99_from_str("Type mismatch: expected "),
                       CharSlice99_from_str((char *)SmolRTSP_ParseType_str(*kind)),
                       CharSlice99_from_str(", found `"),
                       TRUNCATE_STR(*str),
                       CharSlice99_from_str("`."),
                   });
        }
        of(SmolRTSP_ParseError_HeaderMapOverflow) {
            VCALL(w, write, CharSlice99_from_str("Not enough space left in the header map."));
        }
    }
}

#undef MAX_STR
#undef TRUNCATE_STR

SmolRTSP_ParseStatus SmolRTSP_ParseStatus_partial(size_t offset) {
    return (SmolRTSP_ParseStatus){.offset = offset, .is_complete = false};
}

SmolRTSP_ParseStatus SmolRTSP_ParseStatus_complete(size_t offset) {
    return (SmolRTSP_ParseStatus){.offset = offset, .is_complete = true};
}

SmolRTSP_ParseResult SmolRTSP_ParseResult_partial(size_t offset) {
    return SmolRTSP_ParseResult_Success(SmolRTSP_ParseStatus_partial(offset));
}

SmolRTSP_ParseResult SmolRTSP_ParseResult_complete(size_t offset) {
    return SmolRTSP_ParseResult_Success(SmolRTSP_ParseStatus_complete(offset));
}

bool SmolRTSP_ParseResult_is_partial(SmolRTSP_ParseResult self) {
    // Used to workaround `-Wreturn-type`.
    bool result = true;

    match(self) {
        of(SmolRTSP_ParseResult_Success, status) result = !status->is_complete;
        otherwise result = false;
    }

    return result;
}

bool SmolRTSP_ParseResult_is_complete(SmolRTSP_ParseResult self) {
    // Used to workaround `-Wreturn-type`.
    bool result = true;

    match(self) {
        of(SmolRTSP_ParseResult_Success, status) result = status->is_complete;
        otherwise result = false;
    }

    return result;
}
