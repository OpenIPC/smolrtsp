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

#define MAX_STR 10
#define TRUNCATE_STR(str)                                                      \
    ((str).len <= MAX_STR ? (str) : CharSlice99_sub((str), 0, MAX_STR))

int SmolRTSP_ParseError_print(SmolRTSP_ParseError self, SmolRTSP_Writer w) {
    assert(w.self && w.vptr);

    match(self) {
        of(SmolRTSP_ParseError_ContentLength, value) {
            return SMOLRTSP_WRITE_SLICES(
                w, {
                       CharSlice99_from_str("Invalid Content-Length `"),
                       TRUNCATE_STR(*value),
                       CharSlice99_from_str("`"),
                   });
        }
        of(SmolRTSP_ParseError_StrMismatch, expected, actual) {
            return SMOLRTSP_WRITE_SLICES(
                w, {
                       CharSlice99_from_str("String mismatch: expected `"),
                       TRUNCATE_STR(*expected),
                       CharSlice99_from_str("`, found `"),
                       TRUNCATE_STR(*actual),
                       CharSlice99_from_str("`"),
                   });
        }
        of(SmolRTSP_ParseError_TypeMismatch, kind, str) {
            return SMOLRTSP_WRITE_SLICES(
                w,
                {
                    CharSlice99_from_str("Type mismatch: expected "),
                    CharSlice99_from_str((char *)SmolRTSP_ParseType_str(*kind)),
                    CharSlice99_from_str(", found `"),
                    TRUNCATE_STR(*str),
                    CharSlice99_from_str("`"),
                });
        }
        of(SmolRTSP_ParseError_HeaderMapOverflow) {
            return VCALL(
                w, write,
                CharSlice99_from_str(
                    "Not enough space left in the header map"));
        }
        of(SmolRTSP_ParseError_MissingCSeq) {
            return VCALL(w, write, CharSlice99_from_str("Missing CSeq"));
        }
        of(SmolRTSP_ParseError_InvalidCSeq, value) {
            return SMOLRTSP_WRITE_SLICES(
                w, {
                       CharSlice99_from_str("Invalid CSeq `"),
                       TRUNCATE_STR(*value),
                       CharSlice99_from_str("`"),
                   });
        }
    }

    return -1;
}

#undef MAX_STR
#undef TRUNCATE_STR

bool SmolRTSP_ParseStatus_is_complete(SmolRTSP_ParseStatus self) {
    return MATCHES(self, SmolRTSP_ParseStatus_Complete);
}

bool SmolRTSP_ParseStatus_is_partial(SmolRTSP_ParseStatus self) {
    return MATCHES(self, SmolRTSP_ParseStatus_Partial);
}

SmolRTSP_ParseResult SmolRTSP_ParseResult_partial(void) {
    return SmolRTSP_ParseResult_Success(SmolRTSP_ParseStatus_Partial());
}

SmolRTSP_ParseResult SmolRTSP_ParseResult_complete(size_t offset) {
    return SmolRTSP_ParseResult_Success(SmolRTSP_ParseStatus_Complete(offset));
}

bool SmolRTSP_ParseResult_is_success(SmolRTSP_ParseResult self) {
    return MATCHES(self, SmolRTSP_ParseResult_Success);
}

bool SmolRTSP_ParseResult_is_failure(SmolRTSP_ParseResult self) {
    return !SmolRTSP_ParseResult_is_success(self);
}

bool SmolRTSP_ParseResult_is_partial(SmolRTSP_ParseResult self) {
    // Used to workaround `-Wreturn-type`.
    bool result = true;

    match(self) {
        of(SmolRTSP_ParseResult_Success, status) result =
            SmolRTSP_ParseStatus_is_partial(*status);
        otherwise result = false;
    }

    return result;
}

bool SmolRTSP_ParseResult_is_complete(SmolRTSP_ParseResult self) {
    // Used to workaround `-Wreturn-type`.
    bool result = true;

    match(self) {
        of(SmolRTSP_ParseResult_Success, status) result =
            SmolRTSP_ParseStatus_is_complete(*status);
        otherwise result = false;
    }

    return result;
}
