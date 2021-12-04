#include <smolrtsp/error.h>

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

void SmolRTSP_ParseError_print(SmolRTSP_ParseError self, SmolRTSP_UserWriter w, void *user_cx) {
    match(self) {
        of(SmolRTSP_ParseError_ContentLength, value) {
            w(CharSlice99_from_str("Invalid Content-Length `"), user_cx);
            w(*value, user_cx);
            w(CharSlice99_from_str("`."), user_cx);
        }
        of(SmolRTSP_ParseError_StrMismatch, expected, actual) {
            w(CharSlice99_from_str("String mismatch: expected `"), user_cx);
            w(*expected, user_cx);
            w(CharSlice99_from_str("`, found `"), user_cx);
            w(*actual, user_cx);
            w(CharSlice99_from_str("`."), user_cx);
        }
        of(SmolRTSP_ParseError_TypeMismatch, kind, str) {
            w(CharSlice99_from_str("Type mismatch: expected "), user_cx);
            w(CharSlice99_from_str((char *)SmolRTSP_ParseType_str(*kind)), user_cx);
            w(CharSlice99_from_str(", found `"), user_cx);
            w(*str, user_cx);
            w(CharSlice99_from_str("`."), user_cx);
        }
        of(SmolRTSP_ParseError_HeaderMapOverflow) {
            w(CharSlice99_from_str("Not enough space left in the header map."), user_cx);
        }
    }
}

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
